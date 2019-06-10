#include "ballistogram.h"

#include <QDataStream>

Ballistogram::Ballistogram(const QString &chanId, const int freq)
    : SignalData()
    , m_channelId(chanId)
    , m_frequency(freq)
{

}

Ballistogram::Ballistogram(const QByteArray &data)
    : SignalData()
{
    fromByteArray(data);
}

QString Ballistogram::channelId() const
{
    return m_channelId;
}

int Ballistogram::frequency() const
{
    return m_frequency;
}

int Ballistogram::size() const
{
    return m_data.size();
}

int Ballistogram::subChansCount() const
{
    return 1;
}

double Ballistogram::value(const int subChan, const int rec) const
{
    Q_ASSERT(subChan == 0);
    Q_ASSERT(rec >= 0 && rec < size());
    return m_data.at(rec);
}

double Ballistogram::value(const int rec) const
{
    Q_ASSERT(rec >= 0 && rec < size());
    return m_data.at(rec);
}

void Ballistogram::addValue(const double val)
{
    m_data.append(val);
}

void Ballistogram::fromByteArray(const QByteArray &data)
{
    // Формат массива байт с данными о канале
    // [channelId(38)|freq(4)|cnt(4)|V0(double)|V1(double)| ... |VCnt-1(double)]
    //  0          37|38   41|42  45|46  ...

    QByteArray ba = data;
    QDataStream stream(&ba, QIODevice::ReadOnly);
    stream >> m_channelId;
    stream >> m_frequency;

    int count = 0;
    stream >> count;
    m_data.resize(count);

    for (int i = 0; i < count; ++i)
    {
        double val;
        stream >> val;
        m_data.replace(i, val);
    }
}

void Ballistogram::toByteArray(QByteArray &data) const
{
    data.clear();

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_channelId << m_frequency;

    stream << size();
    for (int i = 0; i < size(); ++i)
        stream << m_data.at(i);
}
