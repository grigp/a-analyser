#include "proberesultdata.h"

ProbeResultData::ProbeResultData(const QString &testUid, const QString &uid, const QString &name)
    : m_testUid(testUid)
    , m_uid(uid)
    , m_name(name)
{

}

void ProbeResultData::saveProbe()
{
    foreach (auto* signal, m_signals)
    {
        QByteArray ba;
        signal->toByteArray(ba);
        //todo: массив байт сохранить в БД

        delete signal;
    }
    m_signals.clear();
}

void ProbeResultData::clear()
{
    foreach (auto* signal, m_signals)
        delete signal;
    m_signals.clear();
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
    return m_signals.size();
}

SignalData *ProbeResultData::getSignal(const int num) const
{
    Q_ASSERT(num >= 0 && num < m_signals.size());
    return m_signals.at(num);
}

SignalData *ProbeResultData::getSignal(const QString &channelId) const
{
    foreach (auto *sig, m_signals)
        if (sig->channelId() == channelId)
            return sig;
}

void ProbeResultData::addSignal(SignalData *signal)
{
    m_signals.append(signal);
}
