#include "anysignal.h"

#include <QCoreApplication>
#include <QIODevice>
#include <QUuid>
#include <QDataStream>

AnySignal::AnySignal(const int freq, const int subChans)
    : SignalData()
    , m_frequency(freq)
    , m_subChansCount(subChans)
{

}

QString AnySignal::channelId() const
{
    return m_channelId;
}

void AnySignal::setChannelId(const QString channelId)
{
    m_channelId = channelId;
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

QString AnySignal::subChanName(const int i) const
{
    return QCoreApplication::tr("Канал") + " " + QString::number(i + 1);
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

    //! Минимум и максимум
    foreach (auto val, value)
    {
        if (val > m_maxValue)
            m_maxValue = val;
        if (val < m_minValue)
            m_minValue = val;
    }
}

void AnySignal::fromByteArray(const QByteArray &data)
{
    //! Формат массива байт с данными о канале
    //! [channelId(38)|freq(4)|cnt(4)|sc(4)|V00(double)|V01(double)|...|V0sc-1(double)|V10(double)| ... |VCnt-1sc-1(double)]
    //!  0          37|38   41|42  45|46 49|50  ...

    QByteArray ba = data;
    QDataStream stream(&ba, QIODevice::ReadOnly);
    stream >> m_channelId;
    stream >> m_frequency;

    int count = 0;
    stream >> count;
    m_data.resize(count);
    stream >> m_subChansCount;

    m_maxValue = -INT_MAX;
    m_minValue = INT_MAX;

    for (int i = 0; i < count; ++i)
    {
        QVector<double> rec;
        for (int j = 0; j < m_subChansCount; ++j)
        {
            double val;
            stream >> val;
            rec.append(val);

            //! Минимум и максимум
            if (val > m_maxValue)
                m_maxValue = val;
            if (val < m_minValue)
                m_minValue = val;
        }

        m_data.replace(i, rec);
    }
}

void AnySignal::toByteArray(QByteArray &data) const
{
    data.clear();

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_channelId << m_frequency;

    stream << size() << m_subChansCount;
    for (int i = 0; i < size(); ++i)
        for (int j = 0; j < m_subChansCount; ++j)
            stream << m_data.at(i).at(j);
}

void AnySignal::clear()
{
    foreach (auto val, m_data)
        val.clear();
    m_data.clear();
}
