#include "resultinfo.h"

#include "proberesultdata.h"

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




SignalResultInfo::SignalResultInfo(SignalData* signal)
    : m_signal(signal)
{
}

QString SignalResultInfo::channelId() const
{
    return m_signal->channelId();
}

int SignalResultInfo::frequency() const
{
    return m_signal->frequency();
}

int SignalResultInfo::size() const
{
    return m_signal->size();
}

double SignalResultInfo::value(const int chan, const int rec) const
{
    return m_signal->value(chan, rec);
}
