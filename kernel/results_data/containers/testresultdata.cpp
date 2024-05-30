#include "testresultdata.h"

#include "proberesultdata.h"
#include "resultinfo.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "stabilogram.h"
#include "balistogram.h"
#include "dynamosignal.h"

#include <QUuid>
#include <QDebug>

TestResultData::TestResultData()
{
    m_mode = mdUndefined;
}

void TestResultData::newTest(const QString &patientUid, const QString &metodicUid)
{
    Q_ASSERT(m_mode == mdUndefined);
    m_mode = mdRun;

    m_uid = QUuid::createUuid().toString();
    m_patientUid = patientUid;
    m_metodicUid = metodicUid;
    m_dt = QDateTime::currentDateTime();

    m_probes.clear();
}

void TestResultData::newProbe(const QString &probeName)
{
    Q_ASSERT(m_mode == mdRun);

    ProbeResultData *probe = new ProbeResultData(m_uid, QUuid::createUuid().toString(), probeName);
    m_probes << probe;
}

void TestResultData::saveTest()
{
    Q_ASSERT(m_mode == mdRun);

    auto testUid = DataProvider::addTestStart(m_patientUid, m_metodicUid);

    int n = 0;
    foreach (auto* probe, m_probes)
    {
        probe->saveProbe(testUid, n);
        delete probe;
        ++n;
    }
    m_probes.clear();

    DataProvider::addTestFinish(testUid);

    m_mode = mdUndefined;
}

void TestResultData::openTest(const QString &uid)
{
    Q_ASSERT(m_mode == mdUndefined);
    m_mode = mdHandle;
    m_uid = uid;

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_uid, ti))
    {
        m_params = ti.params;
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            auto probeUid = ti.probes.at(i);

            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(probeUid, pi))
            {
                ProbeResultData *probe = new ProbeResultData(m_uid, probeUid, pi.name);
                m_probes << probe;

                foreach (auto ci, pi.channels)
                {
                    QByteArray chan;
                    DataProvider::getChannel(ci.uid, chan);
                    SignalData *signal = nullptr;
                    if (ci.channelId == ChannelsDefines::chanStab)
                        signal = new Stabilogram(chan);
                    else
                    if (ci.channelId == ChannelsDefines::chanZ)
                        signal = new Balistogram(chan);
                    else
                    if (ci.channelId == ChannelsDefines::chanTargetTraectory)
                        signal = new DecartCoordinatesSignal(chan);
                    else
                    if (ci.channelId == ChannelsDefines::chanMWStickForce)
                        signal = new DynamoSignal(chan);
                    if (signal)
                        probe->addChannel(signal);
                }
            }
        }
    }
}

void TestResultData::closeTest()
{
    foreach (auto* probe, m_probes)
    {
        probe->clear();
        delete probe;
    }
    m_probes.clear();
    m_mode = mdUndefined;
}

void TestResultData::addChannel(ChannelData *channel)
{
    Q_ASSERT(m_mode == mdRun);
    m_probes.at(m_probes.size() - 1)->addChannel(channel);
}

int TestResultData::probesCount() const
{
    return m_probes.size();
}

ProbeResultInfo *TestResultData::probe(const int number) const
{
    Q_ASSERT(number >= 0 && number < m_probes.size());
    return new ProbeResultInfo(m_probes.at(number));
}

QJsonObject TestResultData::getParams() const
{
    Q_ASSERT(m_mode == mdHandle);
    return m_params;
}
