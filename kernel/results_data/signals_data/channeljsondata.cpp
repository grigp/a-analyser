#include "channeljsondata.h"

#include <QJsonDocument>

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
}

void ChannelJsonData::toByteArray(QByteArray &data) const
{
    QJsonDocument doc(m_data);
    data = doc.toJson();
}

void ChannelJsonData::clear()
{
    m_data = QJsonObject();
}
