#include "anysignal.h"

AnySignal::AnySignal(const int freq, const int subChans)
    : SignalData()
    , m_frequency(freq)
    , m_subChansCount(subChans)
{

}

QString AnySignal::channelId() const
{
    return QString("");
}

int AnySignal::frequency() const
{
    return m_frequency;
}

int AnySignal::size() const
{
    return m_data.size();
}

int AnySignal::subChansCount() const
{
    return m_subChansCount;
}

double AnySignal::value(const int subChan, const int rec) const
{
    //! Надо бы контролировать значения subChan и rec, но это может быть тормозная операция
    return m_data.at(rec).at(subChan);
}

void AnySignal::appendValue(QVector<double> &value)
{
    Q_ASSERT(value.size() == m_subChansCount);
    m_data.append(value);
}

void AnySignal::fromByteArray(const QByteArray &data)
{

}

void AnySignal::toByteArray(QByteArray &data) const
{

}

void AnySignal::clear()
{
    foreach (auto val, m_data)
        val.clear();
    m_data.clear();
}
