#include "classicfactors.h"

#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "stabilogram.h"
#include "factorsfactory.h"
#include "aanalyserapplication.h"

#include <QByteArray>
#include <math.h>
#include <QDebug>

ClassicFactors::ClassicFactors(const QString &testUid,
                               const QString &probeUid,
                               const QString &channelId,
                               QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool ClassicFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctStabilogram;
}

bool ClassicFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

void ClassicFactors::calculate()
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), channelId(), baStab))
    {
        Stabilogram stab(baStab);

        //! Смещения
        for (int i = 0; i < stab.size(); ++i)
        {
            auto rec = stab.value(i);
            m_mx = m_mx + rec.x;
            m_my = m_my + rec.y;
        }
        m_mx = m_mx / stab.size();
        m_my = m_my / stab.size();

        //! Разбросы и др.
        double ox = 0;
        double oy = 0;
        for (int i = 0; i < stab.size(); ++i)
        {
            auto rec = stab.value(i);
            m_qx = m_qx + pow(abs(rec.x - m_mx), 2) / (stab.size() - 1);
            m_qy = m_qy + pow(abs(rec.y - m_my), 2) / (stab.size() - 1);
            m_r = m_r + sqrt(pow(rec.x - m_mx, 2) + pow(rec.y - m_my, 2)) / stab.size();

            //! Длина СКГ
            if (i > 0)
                m_l = m_l + sqrt(pow(rec.x - ox, 2) + pow(rec.y - oy, 2));
            ox = rec.x;
            oy = rec.y;
        }
        m_qx = sqrt(m_qx);
        m_qy = sqrt(m_qy);

        //! Эллипс
        computeParamsEllipse(&stab);
    }

    addFactor(ClassicFactorsDefines::MoXUid, m_mx);
    addFactor(ClassicFactorsDefines::MoYUid, m_my);
    addFactor(ClassicFactorsDefines::QXUid, m_qx);
    addFactor(ClassicFactorsDefines::QYUid, m_qy);
    addFactor(ClassicFactorsDefines::RUid, m_r);
    addFactor(ClassicFactorsDefines::LUid, m_l);
    addFactor(ClassicFactorsDefines::AngleUid, m_angle);
    addFactor(ClassicFactorsDefines::SquareUid, m_square);
    addFactor(ClassicFactorsDefines::ComprRatioUid, m_comprRatio);
}

void ClassicFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(ClassicFactorsDefines::GroupUid, tr("Классические показатели"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::MoXUid, ClassicFactorsDefines::GroupUid,
                           tr("Смещение по фронтали"), tr("MoX"), tr("мм"), 2);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::MoYUid, ClassicFactorsDefines::GroupUid,
                           tr("Смещение по сагиттали"), tr("MoY"), tr("мм"), 2);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::QXUid, ClassicFactorsDefines::GroupUid,
                           tr("Разброс по фронтали"), tr("QX"), tr("мм"), 2);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::QYUid, ClassicFactorsDefines::GroupUid,
                           tr("Разброс по сагиттали"), tr("QY"), tr("мм"), 2);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::RUid, ClassicFactorsDefines::GroupUid,
                           tr("Средний радиус"), tr("R"), tr("мм"), 2);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::LUid, ClassicFactorsDefines::GroupUid,
                           tr("Длина статокинезиграммы"), tr("L"), tr("мм"), 0);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::AngleUid, ClassicFactorsDefines::GroupUid,
                           tr("Среднее направление колебаний"), tr("Angle"), tr("град"), 2);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::SquareUid, ClassicFactorsDefines::GroupUid,
                           tr("Площадь эллипса"), tr("EllS"), tr("кв.мм"), 1);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::ComprRatioUid, ClassicFactorsDefines::GroupUid,
                           tr("Коэффициент сжатия"), tr("EllE"), tr(""), 2);
}

void ClassicFactors::computeParamsEllipse(Stabilogram *stab)
{
    QVector<double> rBufX, rBufY;
    for (int i = 0; i < stab->size() - 1; ++i)
    {
        rBufX << stab->value(0, i);
        rBufY << stab->value(1, i);
    }
    double moX = 0;
    double moY = 0;
    double qx = 0;
    double qy = 0;
    meanAndStdDev(rBufX, moX, qx);
    meanAndStdDev(rBufY, moY, qy);

    //! Коэффициент корреляции
    double xy = 0;
    for (int i = 0; i < stab->size() - 1; ++i)
        xy = xy + ((stab->value(0, i) - moX) * (stab->value(1, i) - moY));

    double korr = 0;
    if (qx * qy * stab->size() != 0)
        korr = xy / (qx * qy * stab->size());

    //! Угол поворота
    double alfa = 0.5 * atan2(2 * korr * qx * qy, pow(qx, 2) - pow(qy, 2));

    //! Поворот совокупности точек
    rBufX.clear();
    rBufY.clear();
    for (int i = 0; i < stab->size() - 1; ++i)
    {
        rBufX << ((stab->value(0, i) - moX) * cos(alfa) + (stab->value(1, i) - moY) * sin(alfa));
        rBufY << (-(stab->value(0, i) - moX) * sin(alfa) + (stab->value(1, i) - moY) * cos(alfa));
    }
    //! СКО 0
    double moX0 = 0;
    double moY0 = 0;
    double qx0 = 0;
    double qy0 = 0;
    meanAndStdDev(rBufX, moX0, qx0);
    meanAndStdDev(rBufY, moY0, qy0);

    //! Параметры эллипса
    m_ellipse.sizeB = sqrt(-1 * log(1 - ClassicFactorsDefines::PDotInEllipse)) * qx0;
    m_ellipse.sizeA = sqrt(-2 * log(1 - ClassicFactorsDefines::PDotInEllipse)) * qy0;

    alfa = alfa * (180 / M_PI); ///< Радианы в градусы
    if (alfa < -90)
        m_ellipse.angle = 180 + alfa;
    else
    if (alfa > 90)
        m_ellipse.angle = alfa - 180;
    else
        m_ellipse.angle = - alfa;

    m_ellipse.ellS = m_ellipse.sizeA * m_ellipse.sizeB * M_PI;
    if (m_ellipse.sizeB > m_ellipse.sizeA)
    {
        korr = m_ellipse.sizeB;
        m_ellipse.sizeB = m_ellipse.sizeA;
        m_ellipse.sizeA = korr;
        if (m_ellipse.angle < 0)
            m_ellipse.angle = m_ellipse.angle + 90;
        else
            m_ellipse.angle = m_ellipse.angle - 90;
    }
    m_ellipse.ellE = 0;
    if (m_ellipse.sizeB != 0)
        m_ellipse.ellE = m_ellipse.sizeA / m_ellipse.sizeB;

    m_angle = m_ellipse.angle;
    m_square = m_ellipse.ellS;
    m_comprRatio = m_ellipse.ellE;
}

void ClassicFactors::meanAndStdDev(QVector<double> &data, double &mean, double &stdDev)
{
    mean = 0;
    foreach (auto val, data)
        mean = mean + val;
    if (data.size() > 0)
        mean = mean / data.size();

    stdDev = 0;
    foreach (auto val, data)
        stdDev = stdDev + pow(mean - val, 2);
    if (data.size() - 1 > 0)
        stdDev = sqrt(stdDev / (data.size() - 1));
}