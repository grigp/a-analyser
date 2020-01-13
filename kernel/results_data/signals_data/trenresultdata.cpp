#include "trenresultdata.h"

#include <QJsonArray>
#include <QJsonObject>

TrenResultData::TrenResultData(const QString &chanId)
    : ChannelJsonData(chanId)
{

}

TrenResultData::TrenResultData(const QByteArray &data)
    : ChannelJsonData()
{
    fromByteArray(data);
}

int TrenResultData::size() const
{
    auto arr = data()["factors"].toArray();
    return arr.size();
}

FactorsDefines::FactorValue TrenResultData::factor(const int idx) const
{
    auto arr = data()["factors"].toArray();
    for (int i = 0; i < arr.size(); ++i)
        if (i == idx)
        {
            auto obj = arr.at(i).toObject();
            FactorsDefines::FactorValue fv;
            fv.setUid(obj["uid"].toString());
            fv.setValue(obj["value"].toDouble());
            return fv;
        }

    return FactorsDefines::FactorValue();
}

void TrenResultData::addFactor(const QString &uid, const double value)
{
    QJsonObject obj = data();
    auto arr = obj["factors"].toArray();

    QJsonObject objFactor;
    objFactor["uid"] = uid;
    objFactor["value"] = value;
    arr.append(objFactor);

    obj["factors"] = arr;
    setData(obj);
}
