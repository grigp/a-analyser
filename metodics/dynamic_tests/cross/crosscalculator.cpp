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

    //! Необходимо в тесте, в котором будет возможно ни одна прба найти нужную пробу
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
                    auto factors = new CrossFactors(testUid(), pi.uid);

                    for (int i = 0; i < factors->size(); ++i)
                    {
                        addPrimaryFactor(testUid(), factors->factorUid(i),
                                         factors->factorValue(factors->factorUid(i)),
                                         0, ChannelsDefines::chanStab, pi.name);
                    }

//                    addPrimaryFactor(testUid(), CrossFactorsDefines::LUpUid,
//                                     factors->factorValue(CrossFactorsDefines::LUpUid),
//                                     0, ChannelsDefines::chanStab, pi.name);
//                    addPrimaryFactor(testUid(), CrossFactorsDefines::LDnUid,
//                                     factors->factorValue(CrossFactorsDefines::LDnUid),
//                                     0, ChannelsDefines::chanStab, pi.name);
//                    addPrimaryFactor(testUid(), CrossFactorsDefines::LRtUid,
//                                     factors->factorValue(CrossFactorsDefines::LRtUid),
//                                     0, ChannelsDefines::chanStab, pi.name);
//                    addPrimaryFactor(testUid(), CrossFactorsDefines::LLfUid,
//                                     factors->factorValue(CrossFactorsDefines::LLfUid),
//                                     0, ChannelsDefines::chanStab, pi.name);
//                    addPrimaryFactor(testUid(), CrossFactorsDefines::SZoneUid,
//                                     factors->factorValue(CrossFactorsDefines::SZoneUid),
//                                     0, ChannelsDefines::chanStab, pi.name);
//                    addPrimaryFactor(testUid(), CrossFactorsDefines::UDUid,
//                                     factors->factorValue(CrossFactorsDefines::UDUid),
//                                     0, ChannelsDefines::chanStab, pi.name);
//                    addPrimaryFactor(testUid(), CrossFactorsDefines::RLUid,
//                                     factors->factorValue(CrossFactorsDefines::RLUid),
//                                     0, ChannelsDefines::chanStab, pi.name);
//                    addPrimaryFactor(testUid(), CrossFactorsDefines::SFUid,
//                                     factors->factorValue(CrossFactorsDefines::SFUid),
//                                     0, ChannelsDefines::chanStab, pi.name);
                }
            }
        }
    }

}

void CrossCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}
