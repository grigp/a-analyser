#include "bilateralresultdata.h"

#include "QJsonObject"
#include "QJsonArray"
#include "QVariant"

BilateralResultData::BilateralResultData(const QString &chanId)
    : ChannelJsonData (chanId)
{

}

BilateralResultData::BilateralResultData(const QByteArray &data)
{
    fromByteArray(data);
}

int BilateralResultData::platformsCount() const
{
    return data()["platforms"].toArray().size();
}

QRect BilateralResultData::platform(const int idx) const
{
    auto platforms = data()["platforms"].toArray();
    if (idx >= 0 && idx < platforms.size())
    {
        auto pl = platforms.at(idx).toObject();
        int x = pl["x"].toInt();
        int y = pl["y"].toInt();
        int width = pl["width"].toInt();
        int height = pl["height"].toInt();
        return QRect(x, y, width, height);
    }
    return QRect(0, 0, 0, 0);
}

void BilateralResultData::addPlatform(const QRect &platform)
{
    QJsonObject obj = data();
    auto arr = obj["platforms"].toArray();

    QJsonObject objPlatform;
    objPlatform["x"] = platform.x();
    objPlatform["y"] = platform.y();
    objPlatform["width"] = platform.width();
    objPlatform["height"] = platform.height();
    arr.append(objPlatform);

    obj["platforms"] = arr;
    setData(obj);
}
