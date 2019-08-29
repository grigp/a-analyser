#include "idscalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "idsfactors.h"

IDSCalculator::IDSCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

void IDSCalculator::calculate()
{
    StabTestCalculator::calculate();

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto factors = new IDSFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                addPrimaryFactor(testUid(), IDSFactorsDefines::IDSUid,
                                 factors->factorValue(IDSFactorsDefines::IDSUid),
                                 i, ChannelsDefines::chanStab, pi.name);
                m_factors.append(factors);
            }
        }
    }
}

void IDSCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();

}

IDSFactors *IDSCalculator::factors(const int probeNum) const
{
    Q_ASSERT(probeNum >= 0 && probeNum < m_factors.size());
    return m_factors.at(probeNum);
}
