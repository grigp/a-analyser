#include "vectorfactors.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "stabilogram.h"
#include "channelsutils.h"

#include <math.h>
#include <QDebug>

namespace  {

//! Граничное значение расчета КРИНД
static const int BoundKRIND = 45;

//! Кол-во точек в распределении векторов
static const int RaspredCount = 100;

//! Множитель диапазонов
static const double KoefDiap = 37.5;


}

VectorFactors::VectorFactors(const QString &testUid,
                             const QString &probeUid,
                             const QString &channelId,
                             QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool VectorFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool VectorFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctStabilogram;
}

void VectorFactors::calculate()
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), channelId(), baStab))
    {
        Stabilogram stab(baStab);
        auto r = deviation(&stab);

        //! Только для значимых отклонений, с амплитудой, большей чем 0,5 мм
        if (r >= 0.5)
        {
            //! Расчет скоростей и ускорений по фронтали и сагиттали
            QVector<double> spdX, spdY;
            QVector<double> accelX, accelY;
            computeSpeed(&stab, spdX, spdY, accelX, accelY);

            //! Расчет векторов скорости и угловых скоростей
            QVector<double> spd;
            QVector<double> angles;
            QVector<double> wSpeed;
            vectorSpeed(spdX, spdY, spd, angles, wSpeed);

            //! Расчет КФР
            computeKFR(spd);

            //! Расчет НПВ
            computeNPV(spdX, spdY, stab.frequency());

            computeVariationFactors(spd, stab.frequency(), m_amplV, m_tV);
            computeVariationFactors(wSpeed, stab.frequency(), m_amplW, m_tW);
        }
    }

    addFactor(VectorFactorsDefines::KFRUid, m_kfr);
    addFactor(VectorFactorsDefines::NPVUid, m_npv);
    addFactor(VectorFactorsDefines::KRINDUid, m_krind);
    addFactor(VectorFactorsDefines::VMidUid, m_vMid);
    addFactor(VectorFactorsDefines::AmplVUid, m_amplV);
    addFactor(VectorFactorsDefines::TVUid, m_tV);
    addFactor(VectorFactorsDefines::WMidUid, m_wMid);
    addFactor(VectorFactorsDefines::AmplWUid, m_amplW);
    addFactor(VectorFactorsDefines::TVUid, m_tW);
}

void VectorFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(VectorFactorsDefines::GroupUid, tr("Векторные показатели"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::KFRUid, VectorFactorsDefines::GroupUid,
                           tr("Качество функции равновесия"), tr("КФР"), tr("%"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::NPVUid, VectorFactorsDefines::GroupUid,
                           tr("Нормированная площадь векторограммы"), tr("НПВ"), tr("кв.мм./с"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::KRINDUid, VectorFactorsDefines::GroupUid,
                           tr("Коэф-т резкого изм. напр. движения"), tr("КРИНД"), tr("%"), "");

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::VMidUid, VectorFactorsDefines::GroupUid,
                           tr("Линейная скорость средняя"), tr("ЛСС"), tr("мм/сек"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::AmplVUid, VectorFactorsDefines::GroupUid,
                           tr("Амплитуда вариации линейной скорости"), tr("АВЛС"), tr("мм/сек"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::TVUid, VectorFactorsDefines::GroupUid,
                           tr("Период вариации линейной скорости"), tr("ПВЛС"), tr("сек"), "");

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::WMidUid, VectorFactorsDefines::GroupUid,
                           tr("Угловая скорость средняя"), tr("УСС"), tr("рад/сек"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::AmplWUid, VectorFactorsDefines::GroupUid,
                           tr("Амплитуда вариации угловой скорости"), tr("АВУС"), tr("рад/сек"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::TWUid, VectorFactorsDefines::GroupUid,
                           tr("Период вариации угловой скорости"), tr("ПВУС"), tr("сек"), "");
}

double VectorFactors::deviation(Stabilogram *stab)
{
    double mx = 0;
    double my = 0;
    for (int i = 0; i < stab->size(); ++i)
    {
        auto rec = stab->value(i);
        mx = mx + rec.x;
        my = my + rec.y;
    }
    mx = mx / stab->size();
    my = my / stab->size();

    double r = 0;
    for (int i = 0; i < stab->size(); ++i)
    {
        auto rec = stab->value(i);
        r = r + sqrt(pow(rec.x - mx, 2) + pow(rec.y - my, 2)) / stab->size();
    }
    return r;
}

void VectorFactors::computeSpeed(Stabilogram *stab,
                                 QVector<double> &spdX, QVector<double> &spdY,
                                 QVector<double> &accelX, QVector<double> &accelY)
{
    spdX.clear();
    spdY.clear();
    accelX.clear();
    accelY.clear();

    for (int i = 2; i < stab->size() - 3; ++i)
    {
        //! Расчет скорости с использованием сплайна
        spdX << (stab->value(0, i-2) - 8 * stab->value(0, i-1) + 8 * stab->value(0, i+1) - stab->value(0, i+2)) /
                (12.0 / stab->frequency());
        spdY << (stab->value(1, i-2) - 8 * stab->value(1, i-1) + 8 * stab->value(1, i+1) - stab->value(1, i+2)) /
                (12.0 / stab->frequency());

        accelX << (-stab->value(0, i-2) + 16 * stab->value(0, i-1) - 30 * stab->value(0, i) + 16 * stab->value(0, i+1) - stab->value(0, i+2)) /
                  (12.0 * pow(1/stab->frequency(), 2));
        accelY << (-stab->value(1, i-2) + 16 * stab->value(1, i-1) - 30 * stab->value(1, i) + 16 * stab->value(1, i+1) - stab->value(1, i+2)) /
                  (12.0 * pow(1/stab->frequency(), 2));
    }
}

void VectorFactors::vectorSpeed(const QVector<double> &spdX, const QVector<double> &spdY,
                                QVector<double> &spd, QVector<double> &angles, QVector<double> &wSpeed)
{
    spd.clear();
    angles.clear();
    wSpeed.clear();

    for (int i = 0; i < fmin(spdX.size(), spdY.size()); ++i)
    {
        //! Модуль скорости
        auto v = hypot(spdX.value(i), spdY.value(i));
        spd << v;
        m_vMid = m_vMid + v;

        //! Угол
        angles << atan2(spdY.value(i), spdX.value(i));

        //! Угловая скорость
        if (i > 0)
        {
            auto a1 = atan2(spdY.value(i-1), spdX.value(i-1));
            auto a2 = atan2(spdY.value(i) * cos(a1) - spdX.value(i) * sin(a1),
                            spdX.value(i) * cos(a1) + spdY.value(i) * sin(a1));
            double w = a2 * 180 / M_PI;
            wSpeed << w;
            m_wMid = m_wMid + w;

            //! Расчет КРИНД
            if (w > BoundKRIND)
                m_krind = m_krind + 1;

            //! Кол-во право- и лево- вращений
            if (a2 > 0)
                m_rotRt = m_rotRt + abs(a2);
            else
                m_rotLf = m_rotLf + abs(a2);
        }
    }

    //! Дорасчет VMid
    if (spdX.size() > 0 && spdY.size() > 0)
        m_vMid = m_vMid / fmin(spdX.size(), spdY.size());
    else
        m_vMid = 0;

    //! Дорасчет WMid
    if (wSpeed.size() > 0)
        m_wMid = m_wMid / wSpeed.size();
    else
        m_wMid = 0;

    //! Дорасчет КРИНД
    if (wSpeed.size() > 0)
        m_krind = m_krind / wSpeed.size() * 100;
    else
        m_krind = 0;
}

void VectorFactors::computeKFR(const QVector<double> &spd)
{
    //! Расчет границ диапазонов
    double loV = 0;
    for (int i = 0; i < VectorFactorsDefines::DiapsCount; ++i)
    {
        m_diapazones[i].limitLow = loV;
        m_diapazones[i].limitHigh = sqrt(KoefDiap * (i + 1) / M_PI);
        loV = m_diapazones[i].limitHigh;
        m_diapazones[i].vectorsCnt = 0;
        m_diapazones[i].freq = 0;
        m_diapazones[i].freqAcc = 0;
    }

    //! Расчет распределения векторов
    for (int i = 0; i < spd.size(); ++i)
    {
        auto idx = getDiapasoneIndex(spd.value(i));
        if (idx > -1)
            ++m_diapazones[idx].vectorsCnt;
    }

    //! Расчет частот
    double accF = 0;
    int accVCnt = 0;
    m_diapazones[0].freqAcc = 0;
    for (int i = 0; i < VectorFactorsDefines::DiapsCount; ++i)
    {
        m_diapazones[i].freq = m_diapazones[i].vectorsCnt;
        if (spd.size() > 0)
            m_diapazones[i].freq = m_diapazones[i].freq / spd.size();
        else
            m_diapazones[i].freq = 0;
        accF = accF + m_diapazones[i].freq;
        m_diapazones[i].freqAcc = accF;
        m_diapazones[i].s = ceil(KoefDiap * i);

        accVCnt = accVCnt + m_diapazones[i].vectorsCnt;
        m_diapazones[i].vectorsAcc = accVCnt;
    }

    //! Расчет КФР
    m_kfr = m_diapazones[1].freqAcc / 2;
    for (int i = 1; i < VectorFactorsDefines::DiapsCount; ++i)
        m_kfr = m_kfr + (m_diapazones[i].freqAcc + m_diapazones[i-1].freqAcc) / 2;
    m_kfr = m_kfr / VectorFactorsDefines::DiapsCount * 100;
}

int VectorFactors::getDiapasoneIndex(const double value) const
{
    for (int i = 0; i < VectorFactorsDefines::DiapsCount; ++i)
        if ((value >= m_diapazones[i].limitLow) &&
            (value < m_diapazones[i].limitHigh))
            return i;
    return VectorFactorsDefines::DiapsCount - 1;
}

void VectorFactors::computeNPV(const QVector<double> &spdX,
                               const QVector<double> &spdY,
                               const int frequency)
{
    for (int i = 0; i < fmin(spdX.size() - 1, spdY.size() - 1); ++i)
        m_npv = m_npv + fabs(spdX.value(i) * spdY.value(i+1) -
                             spdX.value(i+1) * spdY.value(i)) / 2;

    if (frequency > 0 && spdX.size() > 0 && spdY.size() > 0)
        m_npv = m_npv / (fmin(spdX.size(), spdY.size()) / frequency);
    else
        m_npv = 0;
}

void VectorFactors::computeVariationFactors(const QVector<double> &spd, const int frequency,
                                            double &amplAv, double &timeAv) const
{
    auto dv = spd.value(1) - spd.value(0);
    auto ve = spd.value(0);
    int ne = 1;
    double ampl = 0;
    int i = 2;
    while (i < spd.size())
    {
        //! Поиск экстремума
        if (dv >= 0)
            while (dv >= 0 && i < spd.size())
            {
                dv = spd.value(i) - spd.value(i-1);
                ++i;
            }
        else
            while (dv <= 0 && i < spd.size())
            {
                dv = spd.value(i) - spd.value(i-1);
                ++i;
            }

        //! Регистрация экстремума
        if (i < spd.size())
        {
            auto i2 = i - 2;
            ampl = ampl + fabs(ve - spd.value(i2));
            ve = spd.value(i2);
            ++ne;
        }
    }

    //! Окончательный расчет амплитуды
    ampl = ampl + fabs(ve - spd.value(i-1));
    amplAv = ampl / ne;

    //! Окончательный расчет периода вариации
    double dne = ne;
    if (ne > 0)
        timeAv = (spd.size()-1) / dne / frequency * 2;
    else
        timeAv = 0;
}
