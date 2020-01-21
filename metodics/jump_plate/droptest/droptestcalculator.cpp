#include "droptestcalculator.h"

#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "droptestfactors.h"

DropTestCalculator::DropTestCalculator(const QString &testUid, QObject *parent)
    : TestCalculator(testUid, parent)
{

}

DropTestCalculator::~DropTestCalculator()
{
    if (m_factors)
        delete m_factors;
}

void DropTestCalculator::calculate()
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
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanDropTestData))
                {
                    m_factors = new DropTestFactors(testUid(), pi.uid, ChannelsDefines::chanDropTestData);

                    addPrimaryFactor(testUid(), DropTestFactorsDefines::JumpsCountUid,
                                     m_factors->factorValue(DropTestFactorsDefines::JumpsCountUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                    addPrimaryFactor(testUid(), DropTestFactorsDefines::FullTimeUid,
                                     m_factors->factorValue(DropTestFactorsDefines::FullTimeUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                    addPrimaryFactor(testUid(), DropTestFactorsDefines::JumpsHeightAvrUid,
                                     m_factors->factorValue(DropTestFactorsDefines::JumpsHeightAvrUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                    addPrimaryFactor(testUid(), DropTestFactorsDefines::JumpsContactTimeAvrUid,
                                     m_factors->factorValue(DropTestFactorsDefines::JumpsContactTimeAvrUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                    addPrimaryFactor(testUid(), DropTestFactorsDefines::JumpsNoContactTimeAvrUid,
                                     m_factors->factorValue(DropTestFactorsDefines::JumpsNoContactTimeAvrUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                }
            }
        }
    }
}

void DropTestCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

int DropTestCalculator::jumpsCount() const
{
    if (m_factors)
        return m_factors->jumpsCount();
    return 0;
}

SignalsDefines::DropJumpFactors DropTestCalculator::jump(const int idx) const
{
    if (m_factors)
        return m_factors->jump(idx);
    return SignalsDefines::DropJumpFactors();
}
