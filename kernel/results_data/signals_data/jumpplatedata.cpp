#include "jumpplatedata.h"



JumpHeightSingleData::JumpHeightSingleData(const QString &chanId)
    : ChannelJsonData(chanId)
{

}

JumpHeightSingleData::JumpHeightSingleData(const QByteArray &data)
    : ChannelJsonData()
{
    fromByteArray(data);
}

double JumpHeightSingleData::height() const
{
    return data()["jump_height"].toDouble();
}

void JumpHeightSingleData::setHeight(const double height)
{
    QJsonObject obj = data();
    obj["jump_height"] = height;
    setData(obj);
}

HoppingData::HoppingData(const QString &chanId)
    : ChannelJsonData(chanId)
{

}

HoppingData::HoppingData(const QByteArray &data)
    : ChannelJsonData()
{
    fromByteArray(data);
}

double HoppingData::time() const
{
    return data()["state_time"].toDouble();
}

void HoppingData::setTime(const double time)
{
    QJsonObject obj = data();
    obj["state_time"] = time;
    setData(obj);
}

double HoppingData::height() const
{
    return data()["jump_height"].toDouble();
}

void HoppingData::setHeight(const double height)
{
    QJsonObject obj = data();
    obj["jump_height"] = height;
    setData(obj);
}
