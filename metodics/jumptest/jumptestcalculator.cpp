#include "jumptestcalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsutils.h"

#include "jumpheightfactors.h"
#include "hoppingfactors.h"

#include <QDebug>

JumpTestCalculator::JumpTestCalculator(const QString &testUid, QObject *parent)
    : TestCalculator(testUid, parent)
{

}

void JumpTestCalculator::calculate()
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
                    m_methodic = JumpTestDefines::MetJumpHeight;
                    auto fct = new JumpHeightFactors(testUid(), pi.uid, ChannelsDefines::chanJumpHeight);
                    addPrimaryFactor(testUid(), JumpHeightFactorsDefines::JumpHeightUid,
                                     fct->factorValue(JumpHeightFactorsDefines::JumpHeightUid),
                                     0, ChannelsDefines::chanJumpHeight, pi.name);
                    qDebug() << "---" << testUid() << JumpHeightFactorsDefines::JumpHeightUid <<
                                         fct->factorValue(JumpHeightFactorsDefines::JumpHeightUid);
                    delete fct;
                }
                else
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanHopping))
                {
                    m_methodic = JumpTestDefines::MetHopping;
                    auto fct = new HoppingFactors(testUid(), pi.uid, ChannelsDefines::chanHopping);
                    addPrimaryFactor(testUid(), HoppingFactorsDefines::JumpHeightUid,
                                     fct->factorValue(HoppingFactorsDefines::JumpHeightUid),
                                     0, ChannelsDefines::chanHopping, pi.name);
                    addPrimaryFactor(testUid(), HoppingFactorsDefines::TimeUid,
                                     fct->factorValue(HoppingFactorsDefines::TimeUid),
                                     0, ChannelsDefines::chanHopping, pi.name);
                    delete fct;
                }
            }
        }
    }
}

void JumpTestCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}