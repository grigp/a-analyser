#include "weighingresultdata.h"

#include <QJsonArray>

WeighingResultData::WeighingResultData(const QString &chanId)
    : ChannelJsonData (chanId)
{

}

WeighingResultData::WeighingResultData(const QByteArray &data)
{
    fromByteArray(data);
}

QTime WeighingResultData::scalingInterval() const
{
    auto sivl = data()["scaling_interval"].toString();
    return QTime::fromString(sivl, "mm:ss");
}

void WeighingResultData::setScalingInterval(const QTime interval)
{
    QJsonObject obj = data();
    obj["scaling_interval"] = interval.toString("mm:ss");
    setData(obj);
}

int WeighingResultData::averageTime() const
{
    return data()["average_time"].toInt();
}

void WeighingResultData::setAverageTime(const int time)
{
    QJsonObject obj = data();
    obj["average_time"] = time;
    setData(obj);
}

int WeighingResultData::size() const
{
    auto arr = data()["weights"].toArray();
    return arr.size();
}

double WeighingResultData::weight(const int idx) const
{
    auto arr = data()["weights"].toArray();
    for (int i = 0; i < arr.size(); ++i)
        if (i == idx)
        {
            auto obj = arr.at(i).toObject();
            return obj["value"].toDouble();
        }
    return 0;
}

void WeighingResultData::addWeight(const double value)
{
    QJsonObject obj = data();
    auto arr = obj["weights"].toArray();

    QJsonObject objValue;
    objValue["value"] = value;
    arr.append(objValue);

    obj["weights"] = arr;
    setData(obj);
}
