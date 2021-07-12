#include "evolventaresultdata.h"

EvolventaResultData::EvolventaResultData(const QString &chanId)
    : ChannelJsonData (chanId)
{

}

EvolventaResultData::EvolventaResultData(const QByteArray &data)
{
    fromByteArray(data);
}

int EvolventaResultData::timeUpwinding() const
{
    return data()["upwinding_time"].toInt();
}

void EvolventaResultData::setTimeUpwinding(const int time)
{
    QJsonObject obj = data();
    obj["upwinding_time"] = time;
    setData(obj);
}

int EvolventaResultData::timeHold() const
{
    return data()["hold_time"].toInt();
}

void EvolventaResultData::setTimeHold(const int time)
{
    QJsonObject obj = data();
    obj["hold_time"] = time;
    setData(obj);
}

int EvolventaResultData::timeConvolution() const
{
    return data()["convolution_time"].toInt();
}

void EvolventaResultData::setTimeConvolution(const int time)
{
    QJsonObject obj = data();
    obj["convolution_time"] = time;
    setData(obj);
}

int EvolventaResultData::freq() const
{
    return data()["freq"].toInt();
}

void EvolventaResultData::setFreq(const int freq)
{
    QJsonObject obj = data();
    obj["freq"] = freq;
    setData(obj);
}

int EvolventaResultData::diap() const
{
    return data()["diap"].toInt();
}

void EvolventaResultData::setDiap(const int diap)
{
    QJsonObject obj = data();
    obj["diap"] = diap;
    setData(obj);
}
