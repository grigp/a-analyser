#include "stressstrategycalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "vectorfactors.h"

StressStrategyCalculator::StressStrategyCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

void StressStrategyCalculator::calculate()
{
    StabTestCalculator::calculate();

    m_factors.clear();
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            if (i > 1)
                break;
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {

                auto factors = new VectorFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                m_factors << factors;
                addPrimaryFactor(testUid(), VectorFactorsDefines::KFRUid,
                                 factors->factorValue(VectorFactorsDefines::KFRUid),
                                 i, ChannelsDefines::chanStab, pi.name);
                if (i == 0)
                    m_fctTarget = factors->factorValue(VectorFactorsDefines::KFRUid);
                else
                if (i == 1)
                    m_fctZones = factors->factorValue(VectorFactorsDefines::KFRUid);
            }
        }
    }
}

void StressStrategyCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();
}

int StressStrategyCalculator::probesCount() const
{
    return m_factors.size();
}

VectorFactors *StressStrategyCalculator::factors(const int probeNum) const
{
    Q_ASSERT(probeNum >= 0 && probeNum < m_factors.size());
    return m_factors.at(probeNum);
}
