#include "octaedroncalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "octaedronfactors.h"

OctaedronCalculator::OctaedronCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void OctaedronCalculator::calculate()
{
    TestCalculator::calculate();

    //! Необходимо в тесте, в котором будет возможно ни одна проба найти нужную пробу
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab) &&
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanOctaedronResult))
                {
                    m_factors = new OctaedronFactors(testUid(), pi.uid);

                    for (int i = 0; i < m_factors->size(); ++i)
                    {
                        addPrimaryFactor(testUid(), m_factors->factorUid(i),
                                         m_factors->factorValue(m_factors->factorUid(i)),
                                         0, ChannelsDefines::chanStab, pi.name);
                    }
                }
            }
        }
    }
}

void OctaedronCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

double OctaedronCalculator::getValue(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < 8);
    if (m_factors)
        return m_factors->factorValue(idx + 1);
    return 0;
}

double OctaedronCalculator::getAverageValue() const
{
    if (m_factors)
        return m_factors->factorValue(OctaedronFactorsDefines::QualityAverageUid);
    return 0;
}

int OctaedronCalculator::stageTime() const
{
    if (m_factors)
        return m_factors->stageTime();
    return 5;
}

QString OctaedronCalculator::circeRoundRuleMode() const
{
    if (m_factors)
        return m_factors->circeRoundRuleMode();
    return BaseDefines::CirceRoundRuleModeValueName.value(BaseDefines::crmRadial);
}

QString OctaedronCalculator::directionMode() const
{
    if (m_factors)
        return m_factors->directionMode();
    return BaseDefines::DirectionModeValueName.value(BaseDefines::dmCounterClockwise);
}

int OctaedronCalculator::radius() const
{
    if (m_factors)
        return m_factors->radius();
    return 50;
}

int OctaedronCalculator::freq() const
{
    if (m_factors)
        return m_factors->freq();
    return 50;
}

int OctaedronCalculator::diap() const
{
    if (m_factors)
        return m_factors->diap();
    return 128;
}
