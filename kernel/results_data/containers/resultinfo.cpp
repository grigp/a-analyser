#include "resultinfo.h"

#include "proberesultdata.h"
#include "signaldata.h"

ProbeResultInfo::ProbeResultInfo(ProbeResultData* probe)
    : m_probe(probe)
{

}

int ProbeResultInfo::signalsCount() const
{
    return m_probe->signalsCount();
}

SignalResultInfo *ProbeResultInfo::signal(const int signalNumber) const
{
    return new SignalResultInfo(m_probe->getSignal(signalNumber));
}

SignalResultInfo *ProbeResultInfo::signal(const QString &channelId) const
{
    return new SignalResultInfo(m_probe->getSignal(channelId));
}

QString ProbeResultInfo::uid() const
{
    return m_probe->uid();
}

QString ProbeResultInfo::name() const
{
    return m_probe->name();
}




SignalResultInfo::SignalResultInfo(ChannelData* channel)
    : m_channel(channel)
{
}

QString SignalResultInfo::channelId() const
{
    return m_channel->channelId();
}

int SignalResultInfo::frequency() const
{
    SignalData* signal = dynamic_cast<SignalData*>(m_channel);
    if (signal)
        return signal->frequency();
    return 0;
}

int SignalResultInfo::size() const
{
    SignalData* signal = dynamic_cast<SignalData*>(m_channel);
    if (signal)
        return signal->size();
    return 0;
}

double SignalResultInfo::value(const int chan, const int rec) const
{
    SignalData* signal = dynamic_cast<SignalData*>(m_channel);
    if (signal)
        return signal->value(chan, rec);
    return 0;
}

double SignalResultInfo::absMaxValue()
{
    SignalData* signal = dynamic_cast<SignalData*>(m_channel);
    if (signal)
        return signal->absMaxValue();
    return 0;
}
