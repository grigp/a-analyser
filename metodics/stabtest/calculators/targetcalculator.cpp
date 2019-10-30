#include "targetcalculator.h"

#include "targetfactors.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"

TargetCalculator::TargetCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

void TargetCalculator::calculate()
{
    StabTestCalculator::calculate();

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        if (ti.probes.size() == 1)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(0), pi))
            {
                m_factors = new TargetFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                addPrimaryFactor(testUid(), TargetFactorsDefines::TargetScoreUid,
                                 m_factors->factorValue(TargetFactorsDefines::TargetScoreUid),
                                 0, ChannelsDefines::chanStab, pi.name);
            }
        }
    }
}

void TargetCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();
}

TargetFactors *TargetCalculator::factors() const
{
    return m_factors;
}
