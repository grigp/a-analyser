#include "stabilogram.h"

#include <QDataStream>

Stabilogram::Stabilogram(const QString &chanId, const int freq)
    : SignalData()
    , m_channelId(chanId)
    , m_frequency(freq)
{

}

Stabilogram::Stabilogram(const QByteArray &data)
    : SignalData()
{
    fromByteArray(data);
}

QString Stabilogram::channelId() const
{
    return m_channelId;
}

int Stabilogram::frequency() const
{
    return m_frequency;
}

int Stabilogram::size() const
{
    return m_data.size();
}

int Stabilogram::subChansCount() const
{
    return 2;
}

double Stabilogram::value(const int subChan, const int rec) const
{
    Q_ASSERT(subChan >= 0 && subChan < subChansCount());
    Q_ASSERT(rec >= 0 && rec < size());
    if (subChan == 0)
        return m_data.at(rec).x;
    else
        return m_data.at(rec).y;
}

SignalsDefines::StabRec Stabilogram::value(const int rec) const
{
    Q_ASSERT(rec >= 0 && rec < size());
    return m_data.at(rec);
}

void Stabilogram::addValue(const SignalsDefines::StabRec rec)
{
    //todo: потенциально тормозное место при постоянных добавлениях
    m_data.append(rec);
}

void Stabilogram::fromByteArray(const QByteArray &data)
{
    // Формат массива байт с данными о канале
    // [channelId(38)|freq(4)|cnt(4)|X0(double)|Y0(double)|X1(double)|Y1(double)| ... |XCnt-1(double)|YCnt-1(double)]
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
        SignalsDefines::StabRec rec;
        stream >> rec.x;
        stream >> rec.y;
        m_data.replace(i, rec);
    }
}

void Stabilogram::toByteArray(QByteArray &data) const
{
    data.clear();

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_channelId << m_frequency;

    stream << size();
    for (int i = 0; i < size(); ++i)
        stream << m_data.at(i).x << m_data.at(i).y;
}
