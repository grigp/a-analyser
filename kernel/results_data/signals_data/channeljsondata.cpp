#include "channeljsondata.h"

#include <QJsonDocument>
#include <QDebug>

ChannelJsonData::ChannelJsonData(const QString &chanId)
    : ChannelData()
    , m_channelId(chanId)
{

}

ChannelJsonData::ChannelJsonData()
    : ChannelData()
{

}

ChannelJsonData::~ChannelJsonData()
{

}

QJsonObject ChannelJsonData::data() const
{
    return m_data;
}

void ChannelJsonData::setData(const QJsonObject data)
{
    m_data = data;
}

void ChannelJsonData::fromByteArray(const QByteArray &data)
{
    QJsonDocument doc(QJsonDocument::fromJson(data));
    m_data = doc.object();
    m_channelId = m_data["channel_id"].toString();
}

void ChannelJsonData::toByteArray(QByteArray &data) const
{
    QJsonObject obj = m_data;
    obj["channel_id"] = m_channelId;
    QJsonDocument doc(obj);
    data = doc.toJson();
}

void ChannelJsonData::clear()
{
    m_data = QJsonObject();
    m_data["channel-id"] = m_channelId;
}
