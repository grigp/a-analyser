#include "jumpheighttestcalculator.h"

#include "channelsdefines.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "jumpheightfactors.h"

JumpHeightTestCalculator::JumpHeightTestCalculator(const QString &testUid, QObject *parent)
    : TestCalculator(testUid, parent)
{

}

JumpHeightTestCalculator::~JumpHeightTestCalculator()
{
    if (m_factors)
        delete m_factors;

}

void JumpHeightTestCalculator::calculate()
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
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanJumpHeight))
                {
                    m_factors = new JumpHeightFactors(testUid(), pi.uid, ChannelsDefines::chanJumpHeight);

                    addPrimaryFactor(testUid(), JumpHeightFactorsDefines::JumpsCountUid,
                                     m_factors->factorValue(JumpHeightFactorsDefines::JumpsCountUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                    addPrimaryFactor(testUid(), JumpHeightFactorsDefines::FullTimeUid,
                                     m_factors->factorValue(JumpHeightFactorsDefines::FullTimeUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                    addPrimaryFactor(testUid(), JumpHeightFactorsDefines::JumpsHeightAvrUid,
                                     m_factors->factorValue(JumpHeightFactorsDefines::JumpsHeightAvrUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                    addPrimaryFactor(testUid(), JumpHeightFactorsDefines::JumpsContactTimeAvrUid,
                                     m_factors->factorValue(JumpHeightFactorsDefines::JumpsContactTimeAvrUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                }
            }
        }
    }
}

void JumpHeightTestCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

int JumpHeightTestCalculator::jumpsCount() const
{
    if (m_factors)
        return m_factors->jumpsCount();
    return 0;
}

SignalsDefines::JumpRec JumpHeightTestCalculator::jump(const int idx) const
{
    if (m_factors)
        return m_factors->jump(idx);
    return SignalsDefines::JumpRec();
}
