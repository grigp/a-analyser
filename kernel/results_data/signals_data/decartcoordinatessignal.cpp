#include "decartcoordinatessignal.h"

#include <QDataStream>

DecartCoordinatesSignal::DecartCoordinatesSignal(const QString &chanId, const int freq)
    : SignalData ()
    , m_channelId(chanId)
    , m_frequency(freq)
{

}

DecartCoordinatesSignal::DecartCoordinatesSignal(const QByteArray &data)
    : SignalData()
{
    fromByteArray(data);
}

QString DecartCoordinatesSignal::channelId() const
{
    return m_channelId;
}

int DecartCoordinatesSignal::frequency() const
{
    return m_frequency;
}

int DecartCoordinatesSignal::size() const
{
    return m_data.size();
}

int DecartCoordinatesSignal::subChansCount() const
{
    return 2;
}

double DecartCoordinatesSignal::value(const int subChan, const int rec) const
{
    Q_ASSERT(subChan >= 0 && subChan < subChansCount());
    Q_ASSERT(rec >= 0 && rec < size());
    if (subChan == 0)
        return m_data.at(rec).x;
    else
        return m_data.at(rec).y;
}

SignalsDefines::StabRec DecartCoordinatesSignal::value(const int rec) const
{
    Q_ASSERT(rec >= 0 && rec < size());
    return m_data.at(rec);
}

void DecartCoordinatesSignal::addValue(const SignalsDefines::StabRec rec)
{
    //! Минимум и максимум
    if (rec.x > m_maxValue)
        m_maxValue = rec.x;
    if (rec.y > m_maxValue)
        m_maxValue = rec.y;
    if (rec.x < m_minValue)
        m_minValue = rec.x;
    if (rec.y < m_minValue)
        m_minValue = rec.y;
    //todo: потенциально тормозное место при постоянных добавлениях
    m_data.append(rec);
}

void DecartCoordinatesSignal::addValue(const double x, const double y)
{
    SignalsDefines::StabRec rec(std::make_tuple(x, y));
    addValue(rec);
}

void DecartCoordinatesSignal::fromByteArray(const QByteArray &data)
{
    //! Формат массива байт с данными о канале
    //! [channelId(38)|freq(4)|cnt(4)|X0(double)|Y0(double)|X1(double)|Y1(double)| ... |XCnt-1(double)|YCnt-1(double)]
    //!  0          37|38   41|42  45|46  ...

    QByteArray ba = data;
    QDataStream stream(&ba, QIODevice::ReadOnly);
    stream >> m_channelId;
    stream >> m_frequency;

    m_maxValue = -INT_MAX;
    m_minValue = INT_MAX;

    int count = 0;
    stream >> count;
    m_data.resize(count);

    for (int i = 0; i < count; ++i)
    {
        SignalsDefines::StabRec rec;
        stream >> rec.x;
        stream >> rec.y;

        //! Минимум и максимум
        if (rec.x > m_maxValue)
            m_maxValue = rec.x;
        if (rec.y > m_maxValue)
            m_maxValue = rec.y;
        if (rec.x < m_minValue)
            m_minValue = rec.x;
        if (rec.y < m_minValue)
            m_minValue = rec.y;

        m_data.replace(i, rec);
    }
}

void DecartCoordinatesSignal::toByteArray(QByteArray &data) const
{
    data.clear();

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_channelId << m_frequency;

    stream << size();
    for (int i = 0; i < size(); ++i)
        stream << m_data.at(i).x << m_data.at(i).y;
}

void DecartCoordinatesSignal::clear()
{
    m_data.clear();
}
