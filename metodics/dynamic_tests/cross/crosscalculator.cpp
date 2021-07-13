#include "crosscalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "crossfactors.h"

CrossCalculator::CrossCalculator(const QString &testUid, QObject *parent)
    : TestCalculator(testUid, parent)
{

}

void CrossCalculator::calculate()
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
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanCrossResult))
                {
                    m_factors = new CrossFactors(testUid(), pi.uid);

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

void CrossCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

int CrossCalculator::diap() const
{
    if (m_factors)
        return m_factors->diap();
    else
        return 0;
}

double CrossCalculator::valueUp() const
{
    if (m_factors)
        return m_factors->factorValue(CrossFactorsDefines::LUpUid);
    else
        return 0;
}

double CrossCalculator::valueDown() const
{
    if (m_factors)
        return m_factors->factorValue(CrossFactorsDefines::LDnUid);
    else
        return 0;
}

double CrossCalculator::valueRight() const
{
    if (m_factors)
        return m_factors->factorValue(CrossFactorsDefines::LRtUid);
    else
        return 0;
}

double CrossCalculator::valueLeft() const
{
    if (m_factors)
        return m_factors->factorValue(CrossFactorsDefines::LLfUid);
    else
        return 0;
}
