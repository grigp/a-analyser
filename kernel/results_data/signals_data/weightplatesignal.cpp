#include "weightplatesignal.h"

#include <QDataStream>
#include <QDebug>

namespace  {
static const int CalcChanCount = 3;

}

WeightPlateSignal::WeightPlateSignal(const QString &chanId, const int freq, const int subChansCount)
    : SignalData ()
    , m_channelId(chanId)
    , m_frequency(freq)
    , m_subChansCount(subChansCount)
{

}

WeightPlateSignal::WeightPlateSignal(const QByteArray &data)
    : SignalData ()
{
    fromByteArray(data);
}

QString WeightPlateSignal::channelId() const
{
    return m_channelId;
}

int WeightPlateSignal::frequency() const
{
    return m_frequency;
}

int WeightPlateSignal::size() const
{
    return m_data.size();
}

int WeightPlateSignal::subChansCount() const
{
    return m_subChansCount;
}

QString WeightPlateSignal::subChanName(const int i) const
{
    if (i < 4)
        return QCoreApplication::tr("Реакция опоры") + " " + QString::number(i + 1);
    else
    if (i == 4)
        return QCoreApplication::tr("Общая масса");
    else
    if (i == 5)
        return QCoreApplication::tr("Отклонение по фронтали");
    else
    if (i == 6)
        return QCoreApplication::tr("Отклонение по сагиттали");
    else
        return "";
}

double WeightPlateSignal::value(const int subChan, const int rec) const
{
    Q_ASSERT(subChan >= 0 && subChan < m_subChansCount);
    Q_ASSERT(rec >= 0 && rec < m_data.size());
    return m_data[rec][subChan];
}

void WeightPlateSignal::addValue(const QVector<double> rec)
{
    m_data << rec;
}

void WeightPlateSignal::fromByteArray(const QByteArray &data)
{
    //! Формат массива байт с данными о канале
    //! [channelId(38)|freq(4)|scc(4)|cnt(4)|V00(double)|V01(double)|...|V0scc(double)|V10(double)|V11(double)|...|V1scc(double)| ... |VCnt-1 0(double)|VCnt-1 1(double)|...|VCnt-1 scc(double)]
    //!  0          37|38   41|42  45|46  50|51 ...
    //! scc - sub chans count

    QByteArray ba = data;
    QDataStream stream(&ba, QIODevice::ReadOnly);
    stream >> m_channelId;
    stream >> m_frequency;
    stream >> m_subChansCount;

    m_maxValue = -INT_MAX;
    m_minValue = INT_MAX;
    for (int i = 0; i < m_subChansCount + CalcChanCount; ++i)
        m_minMax << BaseDefines::MinMaxValue(INT_MAX, -INT_MAX);

    int count = 0;
    stream >> count;
    m_data.resize(count);

    for (int i = 0; i < count; ++i)
    {
        QVector<double> rec;
        for (int j = 0; j < m_subChansCount + CalcChanCount; ++j)
        {
            double v = 0;
            if (j < m_subChansCount)
                stream >> v;
            else
            if (j == m_subChansCount)
            {
                v = 0;
                foreach (auto r, rec)
                    v += r;
            }
            else
            if (j == m_subChansCount + 1)
                v = rec.at(1) + rec.at(2) - rec.at(0) - rec.at(3);
            if (j == m_subChansCount + 2)
                v = rec.at(0) + rec.at(1) - rec.at(2) - rec.at(3);
            rec << v;
            //! Минимум и максимум
            if (v > m_maxValue)
                m_maxValue = v;
            if (v < m_minValue)
                m_minValue = v;
            if (v > m_minMax[j].max)
                m_minMax[j].max = v;
            if (v < m_minMax[j].min)
                m_minMax[j].min = v;
        }

        m_data.replace(i, rec);
    }

    m_subChansCount += CalcChanCount;
}

void WeightPlateSignal::toByteArray(QByteArray &data) const
{
    data.clear();

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_channelId << m_frequency << m_subChansCount;

    stream << size();
    for (int i = 0; i < size(); ++i)
    {
        for (int j = 0; j < m_subChansCount; ++j)
            stream << m_data[i][j];
    }
}

void WeightPlateSignal::clear()
{
    foreach (auto rec, m_data)
        rec.clear();
    m_data.clear();
}

double WeightPlateSignal::maxValueChan(const int subChan) const
{
    Q_ASSERT(subChan >= 0 && subChan < m_minMax.size());
    return m_minMax.at(subChan).max;
}

double WeightPlateSignal::minValueChan(const int subChan) const
{
    Q_ASSERT(subChan >= 0 && subChan < m_minMax.size());
    return m_minMax.at(subChan).min;
}

