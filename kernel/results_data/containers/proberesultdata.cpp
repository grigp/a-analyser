#include "proberesultdata.h"

#include "dataprovider.h"

#include <QDebug>

ProbeResultData::ProbeResultData(const QString &testUid, const QString &uid, const QString &name)
    : m_testUid(testUid)
    , m_uid(uid)
    , m_name(name)
{

}

void ProbeResultData::saveProbe(const QString &testUid, const int step)
{
    auto probeUid = DataProvider::addProbe(testUid, step, m_name);

    foreach (auto* channel, m_channels)
    {
        QByteArray data;
        channel->toByteArray(data);

        DataProvider::addSignal(probeUid, channel->channelId(), data);

        delete channel;
    }
    m_channels.clear();
}

void ProbeResultData::clear()
{
    foreach (auto* channel, m_channels)
        delete channel;
    m_channels.clear();
}

QString ProbeResultData::uid() const
{
    return m_uid;
}

QString ProbeResultData::name() const
{
    return m_name;
}

int ProbeResultData::signalsCount() const
{
    return m_channels.size();
}

ChannelData *ProbeResultData::getSignal(const int num) const
{
    Q_ASSERT(num >= 0 && num < m_channels.size());
    return m_channels.at(num);
}

ChannelData *ProbeResultData::getSignal(const QString &channelId) const
{
    foreach (auto *sig, m_channels)
        if (sig->channelId() == channelId)
            return sig;
    return nullptr;
}

void ProbeResultData::addChannel(ChannelData *signal)
{
    m_channels.append(signal);
}

