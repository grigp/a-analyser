#include "teppingtestcalculator.h"

#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "teppingtestfactors.h"

#include <QDebug>

TeppingTestCalculator::TeppingTestCalculator(const QString &testUid, QObject *parent)
    : TestCalculator(testUid, parent)
{

}

TeppingTestCalculator::~TeppingTestCalculator()
{
    if (m_factors)
        delete m_factors;
}

void TeppingTestCalculator::calculate()
{
    TestCalculator::calculate();

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            if (i > 0)
                break;
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanTeppingData))
                {
                    m_factors = new TeppingTestFactors(testUid(), pi.uid, ChannelsDefines::chanTeppingData);

                    addPrimaryFactor(testUid(), TeppingTestFactorsDefines::StepsCountUid,
                                     m_factors->factorValue(TeppingTestFactorsDefines::StepsCountUid),
                                     0, ChannelsDefines::chanTeppingData, pi.name);
                    addPrimaryFactor(testUid(), TeppingTestFactorsDefines::FullTimeUid,
                                     m_factors->factorValue(TeppingTestFactorsDefines::FullTimeUid),
                                     0, ChannelsDefines::chanTeppingData, pi.name);
                    addPrimaryFactor(testUid(), TeppingTestFactorsDefines::LeftLegNoContactTimeAvrUid,
                                     m_factors->factorValue(TeppingTestFactorsDefines::LeftLegNoContactTimeAvrUid),
                                     0, ChannelsDefines::chanTeppingData, pi.name);
                    addPrimaryFactor(testUid(), TeppingTestFactorsDefines::RightLegNoContactTimeAvrUid,
                                     m_factors->factorValue(TeppingTestFactorsDefines::RightLegNoContactTimeAvrUid),
                                     0, ChannelsDefines::chanTeppingData, pi.name);
                    addPrimaryFactor(testUid(), TeppingTestFactorsDefines::LeftLegContactTimeAvrUid,
                                     m_factors->factorValue(TeppingTestFactorsDefines::LeftLegContactTimeAvrUid),
                                     0, ChannelsDefines::chanTeppingData, pi.name);
                    addPrimaryFactor(testUid(), TeppingTestFactorsDefines::RightLegContactTimeAvrUid,
                                     m_factors->factorValue(TeppingTestFactorsDefines::RightLegContactTimeAvrUid),
                                     0, ChannelsDefines::chanTeppingData, pi.name);
                    addPrimaryFactor(testUid(), TeppingTestFactorsDefines::TempAvrUid,
                                     m_factors->factorValue(TeppingTestFactorsDefines::TempAvrUid),
                                     0, ChannelsDefines::chanTeppingData, pi.name);
                }
            }
        }
    }
}

void TeppingTestCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

int TeppingTestCalculator::stepsCount(BaseUtils::Side side) const
{
    if (m_factors)
        return m_factors->stepsCount(side);
    return 0;
}

SignalsDefines::TeppingStepRec TeppingTestCalculator::step(BaseUtils::Side side, const int idx) const
{
    if (m_factors)
        return m_factors->step(side, idx);
    return SignalsDefines::TeppingStepRec();
}
