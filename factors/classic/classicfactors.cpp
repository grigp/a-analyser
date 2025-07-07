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
                               const int begin,
                               const int end,
                               QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, begin, end, parent)
{
    if (isValid())
        calculate();
}

QString ClassicFactors::uid() const
{
    return ClassicFactorsDefines::GroupUid;
}

QString ClassicFactors::name() const
{
    return nameAsConst();
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

        if (stab.size() > 0)
        {
            //! Смещения
            for (int i = 0; i < stab.size(); ++i)
            {
                auto rec = stab.value(i);
                m_mx += rec.x;
                m_my += rec.y;
            }
            m_mx /= stab.size();
            m_my /= stab.size();

            //! Разбросы и др.
            double ox = 0;
            double oy = 0;
            for (int i = 0; i < stab.size(); ++i)
            {
                auto rec = stab.value(i);
                m_qx += pow(fabs(rec.x - m_mx), 2) / (stab.size() - 1);
                m_qy += pow(fabs(rec.y - m_my), 2) / (stab.size() - 1);
                m_r += sqrt(pow(rec.x - m_mx, 2) + pow(rec.y - m_my, 2)) / stab.size();

                //! Длина СКГ
                if (i > 0)
                    m_l += sqrt(pow(rec.x - ox, 2) + pow(rec.y - oy, 2));
                ox = rec.x;
                oy = rec.y;
            }
            m_qx = sqrt(m_qx);
            m_qy = sqrt(m_qy);

            //! Эллипс
            computeParamsEllipse(&stab);

            //! LFS после эллипса
            m_lfs = m_l / m_square;
        }
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
    addFactor(ClassicFactorsDefines::EllLengthUid, m_ellLength);
    addFactor(ClassicFactorsDefines::EllWidthUid, m_ellWidth);
    addFactor(ClassicFactorsDefines::LFSUid, m_lfs);
}

void ClassicFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(ClassicFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::MoXUid, ClassicFactorsDefines::GroupUid,
                           tr("Смещение по фронтали"), tr("MoX"), tr("мм"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::MoYUid, ClassicFactorsDefines::GroupUid,
                           tr("Смещение по сагиттали"), tr("MoY"), tr("мм"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::QXUid, ClassicFactorsDefines::GroupUid,
                           tr("Разброс по фронтали"), tr("QX"), tr("мм"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::QYUid, ClassicFactorsDefines::GroupUid,
                           tr("Разброс по сагиттали"), tr("QY"), tr("мм"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::RUid, ClassicFactorsDefines::GroupUid,
                           tr("Средний радиус"), tr("R"), tr("мм"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::LUid, ClassicFactorsDefines::GroupUid,
                           tr("Длина статокинезиграммы"), tr("L"), tr("мм"), 0, 2, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::AngleUid, ClassicFactorsDefines::GroupUid,
                           tr("Среднее направление колебаний"), tr("Angle"), tr("град"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::SquareUid, ClassicFactorsDefines::GroupUid,
                           tr("Площадь эллипса"), tr("EllS"), tr("кв.мм"), 1, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::ComprRatioUid, ClassicFactorsDefines::GroupUid,
                           tr("Коэффициент сжатия"), tr("EllE"), tr(""), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::EllLengthUid, ClassicFactorsDefines::GroupUid,
                           tr("Длина эллипса"), tr("EllL"), tr("мм"), 1, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::EllWidthUid, ClassicFactorsDefines::GroupUid,
                           tr("Ширина эллипса"), tr("EllW"), tr("мм"), 1, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(ClassicFactorsDefines::LFSUid, ClassicFactorsDefines::GroupUid,
                           tr("Длина в зависимости от площади"), tr("LFS"), tr("1/мм"), 1, 2, FactorsDefines::nsDual, 12);

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
    if (fabs(qx * qy * stab->size()) > 0)
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
    if (fabs(m_ellipse.sizeB) > 0)
        m_ellipse.ellE = m_ellipse.sizeA / m_ellipse.sizeB;

    m_angle = m_ellipse.angle;
    m_square = m_ellipse.ellS;
    m_comprRatio = m_ellipse.ellE;
    m_ellLength = m_ellipse.sizeA * 2;
    m_ellWidth = m_ellipse.sizeB * 2;
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

QString ClassicFactors::nameAsConst()
{
    return QCoreApplication::tr("Классические показатели");
}
