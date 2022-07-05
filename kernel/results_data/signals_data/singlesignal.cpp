#include "singlesignal.h"

#include <QDataStream>
#include <QDebug>


SingleSignal::SingleSignal(const QString &chanId, const int freq)
    : SignalData()
    , m_channelId(chanId)
    , m_frequency(freq)
{

}

SingleSignal::SingleSignal(const QByteArray &data)
    : SignalData()
{
    fromByteArray(data);
}

QString SingleSignal::channelId() const
{
    return m_channelId;
}

int SingleSignal::frequency() const
{
    return m_frequency;
}

int SingleSignal::size() const
{
    return m_data.size();
}

int SingleSignal::subChansCount() const
{
    return 1;
}

QString SingleSignal::subChanName(const int i) const
{
    Q_UNUSED(i);
    return "";
}

double SingleSignal::value(const int subChan, const int rec) const
{
    Q_ASSERT(subChan == 0);
    Q_ASSERT(rec >= 0 && rec < size());
    return m_data.at(rec);
}

double SingleSignal::value(const int rec) const
{
    Q_ASSERT(rec >= 0 && rec < size());
    return m_data.at(rec);
}

void SingleSignal::addValue(const double val)
{
    m_data.append(val);
}

void SingleSignal::fromByteArray(const QByteArray &data)
{
    //! Формат массива байт с данными о канале
    //! [channelId(38)|freq(4)|cnt(4)|V0(double)|V1(double)| ... |VCnt-1(double)]
    //!  0          37|38   41|42  45|46  ...

    QByteArray ba = data;
    QDataStream stream(&ba, QIODevice::ReadOnly);
    stream >> m_channelId;
    stream >> m_frequency;

    int count = 0;
    stream >> count;
    m_data.resize(count);

    m_maxValue = -INT_MAX;
    m_minValue = INT_MAX;
    m_midValue = 0;

    for (int i = 0; i < count; ++i)
    {
        double val;
        stream >> val;

        //! Минимум и максимум
        if (val > m_maxValue)
            m_maxValue = val;
        if (val < m_minValue)
            m_minValue = val;
        m_midValue += val;

        m_data.replace(i, val);
    }

    if (count > 0)
        m_midValue /= count;
}

void SingleSignal::toByteArray(QByteArray &data) const
{
    data.clear();

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_channelId << m_frequency;

    stream << size();
    for (int i = 0; i < size(); ++i)
        stream << m_data.at(i);
}

void SingleSignal::clear()
{
    m_data.clear();
}
