#include "proberesultdata.h"

ProbeResultData::ProbeResultData(const QString &uid, const QString &name)
    : m_uid(uid)
    , m_name(name)
{

}

QString ProbeResultData::uid()
{
    return m_uid;
}

QString ProbeResultData::name()
{
    return m_name;
}

int ProbeResultData::signalsCount()
{
    return m_signals.size();
}

SignalData *ProbeResultData::getSignal(const int num)
{
    Q_ASSERT(num >= 0 && num < m_signals.size());
    return m_signals.at(num);
}

SignalData *ProbeResultData::getSignal(const QString &channelId)
{
    foreach (auto *sig, m_signals)
        if (sig->channelId() == channelId)
            return sig;
}
