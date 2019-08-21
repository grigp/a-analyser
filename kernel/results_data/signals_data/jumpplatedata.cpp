#include "jumpplatedata.h"



JumpHeightData::JumpHeightData(const QString &chanId)
    : ChannelJsonData()
    , m_channelId(chanId)
{

}

QString JumpHeightData::channelId() const
{
    return m_channelId;
}

double JumpHeightData::height() const
{
    return data()["jump_height"].toDouble();
}

void JumpHeightData::setHeight(const double height)
{
    QJsonObject obj;
    obj["jump_height"] = height;
    setData(obj);
}

HoppingData::HoppingData(const QString &chanId)
    : ChannelJsonData()
    , m_channelId(chanId)
{

}

QString HoppingData::channelId() const
{
    return m_channelId;
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
