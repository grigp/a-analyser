#include "myogram.h"

#include <QIODevice>
#include <QDataStream>

Myogram::Myogram(const QString &chanId, const int subChansCount, const int freq)
    : SignalData()
    , m_channelId(chanId)
    , m_subChansCount(subChansCount)
    , m_frequency(freq)
{

}

Myogram::Myogram(const QByteArray &data)
    : SignalData()
{
    fromByteArray(data);
}

QString Myogram::channelId() const
{
    return m_channelId;
}

int Myogram::frequency() const
{
    return m_frequency;
}

int Myogram::size() const
{
    return m_data.size();
}

int Myogram::subChansCount() const
{
    return m_subChansCount;
}

double Myogram::value(const int subChan, const int rec) const
{
    Q_ASSERT(subChan >= 0 && subChan < m_subChansCount);
    Q_ASSERT(rec > 0 && rec < m_data.size());
    return m_data.at(rec).data.at(subChan);
}

QVector<double> Myogram::value(const int rec) const
{
    Q_ASSERT(rec > 0 && rec < m_data.size());
    return m_data.at(rec).data;
}

void Myogram::addValue(const QVector<double> rec)
{
    Q_ASSERT(rec.size() == m_subChansCount);
    SignalsDefines::MyoRec myoRec(rec);
    m_data.append(myoRec);
}

void Myogram::fromByteArray(const QByteArray &data)
{
    //! Формат массива байт с данными о канале
    //! [channelId(38)|freq(4)|cnt(4)|sub_chan_cnt(4)|MYO00(double)|...|MYO0N(double)|MYO10(double)|...|MYO1N(double)| ... |MYOCnt-1 0(double)|...|MYOCnt-1 N(double)]
    //!  0          37|38   41|42  45|46           49|50  ...

    QByteArray ba = data;
    QDataStream stream(&ba, QIODevice::ReadOnly);
    stream >> m_channelId;
    stream >> m_frequency;
    stream >> m_subChansCount;

    m_maxValue = -INT_MAX;
    m_minValue = INT_MAX;

    int count = 0;
    stream >> count;
    m_data.resize(count);

    for (int i = 0; i < count; ++i)
    {
        SignalsDefines::MyoRec rec;
        rec.data.resize(m_subChansCount);
        for (int j = 0; j < m_subChansCount; ++j)
        {
            stream >> rec.data[j];

            //! Минимум и максимум
            if (rec.data[j] > m_maxValue)
                m_maxValue = rec.data[j];
            if (rec.data[j] < m_minValue)
                m_minValue = rec.data[j];
        }

        m_data.replace(i, rec);
    }
}

void Myogram::toByteArray(QByteArray &data) const
{
    data.clear();

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_channelId << m_frequency << m_subChansCount;

    stream << size();
    for (int i = 0; i < size(); ++i)
        for (int j = 0; j < m_data.at(i).data.size(); ++j)
            stream << m_data.at(i).data.at(j);
}

void Myogram::clear()
{
    m_data.clear();
}

