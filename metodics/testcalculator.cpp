#include "testcalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsutils.h"

#include <QUuid>
#include <QDebug>

TestCalculator::TestCalculator(const QString &testUid, QObject *parent)
    : QObject(parent)
    , m_testUid(testUid)
{

}

void TestCalculator::calculate()
{
//    computeTestTree();
}

void TestCalculator::fastCalculate()
{

}

FactorsDefines::FactorValueDescript *TestCalculator::primaryFactor(const int num) const
{
    Q_ASSERT(num >= 0 && num < m_primaryFactors.size());
    return m_primaryFactors.at(num);
}

//void TestCalculator::computeTestTree()
//{
//    m_mdlTest.clear();
//    DataDefines::TestInfo ti;
//    if (DataProvider::getTestInfo(m_testUid, ti))
//    {
//        auto *testItem = new QStandardItem(ti.uid);
//        testItem->setData(LevelTest, LevelRole);
//        testItem->setData(ti.uid, TestUidRole);
//        testItem->setData(ti.dateTime, TestDateTimeRole);
//        testItem->setData(ti.patientUid, PatientUidRole);
//        testItem->setData(ti.metodUid, MetodicUidRole);
//        testItem->setData(ti.params, ParamsRole);

//        m_mdlTest.appendRow(testItem);

//        foreach (auto probeUid, ti.probes)
//        {
//            DataDefines::ProbeInfo pi;
//            if (DataProvider::getProbeInfo(probeUid, pi))
//            {
//                QList<MetodicDefines::MetodicInfo> ml = DataProvider::getListMetodisc();
//                QString metName = "";
//                foreach (auto mi, ml)
//                    if (mi.uid == ti.metodUid)
//                        metName = mi.name;
//                auto *probeItem = new QStandardItem(metName);
//                probeItem->setData(LevelProbe, LevelRole);
//                probeItem->setData(pi.uid, ProbeUidRole);
//                probeItem->setData(pi.step, ProbeStepRole);
//                testItem->appendRow(probeItem);

//                foreach (auto ci, pi.channels)
//                {
//                    auto *channelItem = new QStandardItem(ChannelsUtils::instance().channelName(ci.channelId));
//                    channelItem->setData(LevelChannel, LevelRole);
//                    channelItem->setData(ci.channelId, ChannelIdRole);
//                    channelItem->setData(ci.uid, ChannelUidRole);
//                    testItem->appendRow(channelItem);
//                }
//            }
//        }
//    }
//}

void TestCalculator::addPrimaryFactor(const QString &uid,
                                      const double value,
                                      const int probeNum,
                                      const QString &channelId,
                                      const QString &description)
{
    m_primaryFactors.append(new FactorsDefines::FactorValueDescript(uid, value, probeNum, channelId, description));
}

void TestCalculator::addPrimaryFactor(const QString &uid, const double value, const QString &description)
{
    addPrimaryFactor(uid, value, -1, QUuid().toString(), description);
}
