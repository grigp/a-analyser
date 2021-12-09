#include "stepdeviationfactors.h"

#include "dataprovider.h"
#include "channelsdefines.h"
#include "metodicsfactory.h"
#include "aanalyserapplication.h"
#include "stabilogram.h"

#include <QDebug>

StepDeviationFactors::StepDeviationFactors(const QString &testUid,
                                         const QString &probeUid,
                                         QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

bool StepDeviationFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool StepDeviationFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);

    bool te = false;
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        //! uid шаблона методики
        MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
        auto mi = metFactory->metodic(ti.metodUid);
        te = (mi.templateId == MetodicDefines::MetUid_StepDeviatoin);
    }

    return de && te;
}

void StepDeviationFactors::calculate()
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        //! Направление
        auto d = ti.params["direction"].toString();
        m_direction = BaseUtils::DirectionValueIndex.value(d);

        readSignal();
        assignSections();
        computeFactorsMain();
        computeFactorsExtremums();
        addFactors();
    }
}

void StepDeviationFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(StepDeviationFactorsDefines::GroupUid, tr("Показатели теста прироста"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::TimeUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Длительность пробы"), tr("Time"), tr("сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::StepCountUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Количество шагов"), tr("Steps"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::ErrorCountUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Количество ошибок"), tr("Errors"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::GrowthAvrgUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Средняя величина пророста"), tr("Growth"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::TimeAvrgUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Среднее время отклонений"), tr("TimeAvrg"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::SensitivityUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Порог чувствительности"), tr("Sens"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::MaxDeviationUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Максимальное отклонение"), tr("MaxDev"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::DifferenceUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Дифференциация"), tr("Differ"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::TimeIncreaseAwrgUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Среднее время нарастания"), tr("TimeIncrAv"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::TimeReturnAwrgUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Среднее время возврата"), tr("TimeRetAv"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::MinExtrTimeUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Минимальное время между экстремумами"), tr("TimeExtMin"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::MaxExtrTimeUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Максимальное время между экстремумами"), tr("TimeExtMax"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::PickCountUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Общее кол-во \"пиков\" на экстремумах"), tr("PickCnt"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::PickCountAvrgUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Среднее кол-во \"пиков\" на экстремумах"), tr("PickCntAvrg"), tr(""), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::MaxPickCountUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Максимальное кол-во \"пиков\" на экстремумах"), tr("PickCntMax"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::MinPickCountUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Минимальное кол-во \"пиков\" на экстремумах"), tr("PickCntMin"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::ExtrPickCountUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Кол-во пиков с экстремумами"), tr("ExtPickCnt"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::AmplSemiWaveUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Амплитуда полуволны на экстремуме"), tr("ExtWaveAmp"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepDeviationFactorsDefines::TimeSemiWaveUid, StepDeviationFactorsDefines::GroupUid,
                           tr("Длительность полуволны на экстремуме"), tr("ExtWaveTm"), tr("сек"), 3, 3, FactorsDefines::nsDual, 12);
}

int StepDeviationFactors::signalSize() const
{
    return m_signal.size();
}

double StepDeviationFactors::signal(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_signal.size());
    return m_signal.at(idx);
}

double StepDeviationFactors::signalFiltred(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_signalFlt.size());
    return m_signalFlt.at(idx);
}

int StepDeviationFactors::growthDynCount() const
{
    return m_growthDynamic.size();
}

double StepDeviationFactors::growthDynValue(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_growthDynamic.size());
    return  m_growthDynamic.at(idx);
}

int StepDeviationFactors::lengthDynCount() const
{
    return m_lengthDynamic.size();
}

double StepDeviationFactors::lengthDynValue(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_lengthDynamic.size());
    return  m_lengthDynamic.at(idx);
}

void StepDeviationFactors::readSignal()
{
    m_signal.clear();
    m_signalFlt.clear();

    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);
        m_freq = stab.frequency();

        for (int i = 0; i < stab.size(); ++i)
        {
            double v = 0;
            if (m_direction == BaseUtils::dirUp)
                v = stab.value(1, i);
            else
            if (m_direction == BaseUtils::dirDown)
                v = -stab.value(1, i);
            else
            if (m_direction == BaseUtils::dirRight)
                v = stab.value(0, i);
            else
            if (m_direction == BaseUtils::dirLeft)
                v = -stab.value(0, i);

            m_signal << v;
            m_signalFlt << v;
        }

        //! Фильтрация
        BaseUtils::filterLowFreq(m_signalFlt, stab.frequency(), 1, BaseUtils::fkChebyshev, 0, m_signalFlt.size() - 1);
        BaseUtils::swapVector(m_signalFlt);
        BaseUtils::filterLowFreq(m_signalFlt, stab.frequency(), 1, BaseUtils::fkChebyshev, 0, m_signalFlt.size() - 1);
        BaseUtils::swapVector(m_signalFlt);
    }
}

void StepDeviationFactors::assignSections()
{
    //! Построение списка экстремумов
    m_extrList.clear();

    if (m_signalFlt.size() < 10)
        return;

    int dir = 0;
    auto setDir = [&](const int i)
    {
        if (m_signalFlt[i] > m_signalFlt[i - 1])
            dir = 1;
        else
        if (m_signalFlt[i] < m_signalFlt[i - 1])
            dir = -1;
    };
    setDir(1);

    for (int i = 2; i < m_signalFlt.size(); ++i)
    {
        if ((m_signalFlt[i] - m_signalFlt[i-1] > 0) && (dir == -1))
        {
            BaseUtils::Extremum extr(i, BaseUtils::Minimum);
            m_extrList << extr;
        }
        if ((m_signalFlt[i] - m_signalFlt[i-1] < 0) && (dir == 1))
        {
            BaseUtils::Extremum extr(i, BaseUtils::Maximum);
            m_extrList << extr;
        }

        setDir(i);
    }

    //! Построение списка участков экстремумов
    m_Extr.clear();
    for (int i = 0; i < m_extrList.size(); ++i)
    {
        int p = 0;
        int c = m_extrList.at(i).pos;
        int n = m_signal.size() - 1;
        if (i > 0)
            p = m_extrList.at(i-1).pos;
        if (i < m_extrList.size() - 1)
            n = m_extrList.at(i+1).pos;
        int begin = static_cast<int>(c - (c - p) * 0.25);
        int end = static_cast<int>(c + (n - c) * 0.25);
        BaseUtils::Section sec(begin, end);
        m_Extr << sec;
    }

    //! Построение списка участков переходов
    m_Trans.clear();
    for (int i = 0; i < m_Extr.size() - 1; ++i)
    {
        int begin = m_Extr.at(i).end;
        int end = m_Extr.at(i+1).begin;
        BaseUtils::Section sec(begin, end);
        m_Trans << sec;
    }
}

void StepDeviationFactors::computeFactorsMain()
{
    //! Время теста
    m_time = m_signal.size() / m_freq;

    double m = 0;
    double q = 0;
    computeStartPosition(m, q);

    //! Показатели, связанные с экстремумами
    m_stepCount = 0;
    double lastV = -INT_MAX;
    int lastExtrPos = 0;
    m_maxDeviation = -INT_MAX;
    int lastPos = 0;
    int firstStepIdx = -1;
    int retCnt = 0;
    m_minExtrTime = INT_MAX;
    m_maxExtrTime = -INT_MAX;

    m_growthDynamic.clear();
    m_lengthDynamic.clear();

    for (int i = 0; i < m_extrList.size(); ++i)
    {
        if (m_extrList.at(i).kind == BaseUtils::Maximum)
        {
            //! Кол-во шагов
            ++m_stepCount;
            //! Кол-во ошибок
            if (m_signal.value(m_extrList.at(i).pos) <= lastV)
                ++m_errorCount;

            if (m_stepCount > 1)
            {
                //! Средняя величина прироста
                double gv = m_signal.value(m_extrList.at(i).pos) - lastV;
                m_growthAvrg += gv;
                m_growthDynamic << gv;
                //! Среднее время отклонений
                double lv = m_extrList.at(i).pos - lastPos;
                m_timeAvrg += lv;
                m_lengthDynamic << lv;
            }

            //! Порог чувствительности
            if (m_signal.value(m_extrList.at(i).pos) > m + q && firstStepIdx == -1)
                firstStepIdx = i;

            //! Максимальное отклонение
            if (m_signal.value(m_extrList.at(i).pos) > m_maxDeviation)
                m_maxDeviation = m_signal.value(m_extrList.at(i).pos);

            lastV = m_signal.value(m_extrList.at(i).pos);
            lastPos = m_extrList.at(i).pos;

            m_timeIncreaseAwrg += (m_extrList.at(i).pos - lastExtrPos);
        }
        if (m_extrList.at(i).kind == BaseUtils::Minimum)
        {
            m_timeReturnAwrg += (m_extrList.at(i).pos - lastExtrPos);
            ++retCnt;
        }

        if (m_extrList.at(i).pos - lastExtrPos < m_minExtrTime)
            m_minExtrTime = m_extrList.at(i).pos - lastExtrPos;
        if (m_extrList.at(i).pos - lastExtrPos > m_maxExtrTime)
            m_maxExtrTime = m_extrList.at(i).pos - lastExtrPos;

        lastExtrPos = m_extrList.at(i).pos;
    }

    m_growthAvrg /= m_stepCount;
    m_timeAvrg /= (m_freq * m_stepCount);
    m_sensitivity = m_signal.value(m_extrList.at(firstStepIdx).pos);
    m_timeIncreaseAwrg /= (m_freq * m_stepCount);
    m_timeReturnAwrg /= (m_freq * retCnt);
    m_minExtrTime /= m_freq;
    m_maxExtrTime /= m_freq;

    //! Дифференцация
    if ((m_stepCount - m_errorCount > 0) && (m_stepCount > m_errorCount))
    {
        if (m_maxDeviation >= m_sensitivity)
            m_difference = (m_maxDeviation - m_sensitivity) / (m_stepCount - m_errorCount);
        else
            m_difference = m_sensitivity / (m_stepCount - m_errorCount);
    }
    else
        m_difference = 0;
}

void StepDeviationFactors::computeFactorsExtremums()
{

    m_maxPickCount = -INT_MAX;
    m_minPickCount = INT_MAX;
    foreach (auto extr, m_Extr)
    {
        int pickCount = 0;
        double ampl = 0;
        double time = 0;
        computePicks(extr.begin, extr.end, pickCount, ampl, time);

        if (pickCount > 0)
            ++m_extrPickCount;
        m_pickCount += pickCount;
        if (pickCount > m_maxPickCount)
            m_maxPickCount = pickCount;
        if (pickCount < m_minPickCount)
            m_minPickCount = pickCount;
        m_amplSemiWave += ampl;
        m_timeSemiWave += time;
    }
    m_pickCountAvrg = m_pickCount / m_Extr.size();
    m_amplSemiWave /= m_Extr.size();
    m_timeSemiWave /= m_Extr.size();
}

void StepDeviationFactors::computePicks(const int begin, const int end, int &pickCount, double &ampl, double &time)
{
    int dir = 0;
    int li = begin;
    double lv = 0;
    for (int i = begin; i < end; ++i)
    {
        double v = m_signal.at(i);

        if (i > begin)
        {
            double vp = m_signal.at(i-1);
            if((v < vp && dir != 1) ||
               (v > vp && dir != -1))
            {
                ++pickCount;

                if (pickCount > 1)
                {
                    ampl += fabs(v - lv);
                    time += (i - li);
                }
                li = i;
                lv = v;
            }
        }
    }

    if (pickCount > 1)
    {
        ampl /= (pickCount - 1);
        time /= (pickCount - 1);
    }
    if (pickCount > 0)
        --pickCount;
}


void StepDeviationFactors::computeStartPosition(double &m, double &q)
{
    BaseUtils::MidAndStandardDeviation msd;
    //! Среднее за 1 сек от начала
    for (int i = 0; i < m_signal.size() && i < m_freq; ++i)
        msd.add(m_signal[i]);
    m = 0;
    q = 0;
    msd.calculate(m, q);
}

void StepDeviationFactors::addFactors()
{
    addFactor(StepDeviationFactorsDefines::TimeUid, m_time);
    addFactor(StepDeviationFactorsDefines::StepCountUid, m_stepCount);
    addFactor(StepDeviationFactorsDefines::ErrorCountUid, m_errorCount);
    addFactor(StepDeviationFactorsDefines::GrowthAvrgUid, m_growthAvrg);
    addFactor(StepDeviationFactorsDefines::TimeAvrgUid, m_timeAvrg);
    addFactor(StepDeviationFactorsDefines::SensitivityUid, m_sensitivity);
    addFactor(StepDeviationFactorsDefines::MaxDeviationUid, m_maxDeviation);
    addFactor(StepDeviationFactorsDefines::DifferenceUid, m_difference);
    addFactor(StepDeviationFactorsDefines::TimeIncreaseAwrgUid, m_timeIncreaseAwrg);
    addFactor(StepDeviationFactorsDefines::TimeReturnAwrgUid, m_timeReturnAwrg);
    addFactor(StepDeviationFactorsDefines::MinExtrTimeUid, m_minExtrTime);
    addFactor(StepDeviationFactorsDefines::MaxExtrTimeUid, m_maxExtrTime);
    addFactor(StepDeviationFactorsDefines::PickCountUid, m_pickCount);
    addFactor(StepDeviationFactorsDefines::PickCountAvrgUid, m_pickCountAvrg);
    addFactor(StepDeviationFactorsDefines::MaxPickCountUid, m_maxPickCount);
    addFactor(StepDeviationFactorsDefines::MinPickCountUid, m_minPickCount);
    addFactor(StepDeviationFactorsDefines::ExtrPickCountUid, m_extrPickCount);
    addFactor(StepDeviationFactorsDefines::AmplSemiWaveUid, m_amplSemiWave);
    addFactor(StepDeviationFactorsDefines::TimeSemiWaveUid, m_timeSemiWave);

}

