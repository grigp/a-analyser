#include "takeputresultdata.h"

#include <QJsonObject>
#include <QJsonArray>

TakePutResultData::TakePutResultData(const QString &chanId)
    : ChannelJsonData (chanId)
{

}

TakePutResultData::TakePutResultData(const QByteArray &data)
{
    fromByteArray(data);
}

int TakePutResultData::eventCount() const
{
    auto arr = data()["events"].toArray();
    return arr.size();
}

TakePutResultData::Event TakePutResultData::event(const int idx) const
{
    auto arr = data()["events"].toArray();
    for (int i = 0; i < arr.size(); ++i)
        if (i == idx)
        {
            auto obj = arr.at(i).toObject();
            Event ev(obj["counter"].toInt(), static_cast<Kind>(obj["kind"].toInt()));
            return ev;
        }

    return Event(-1, tpkNone);
}

void TakePutResultData::addEvent(const int counter, const TakePutResultData::Kind kind)
{
    QJsonObject obj = data();
    auto arr = obj["events"].toArray();

    QJsonObject objEvent;
    objEvent["kind"] = kind;
    objEvent["counter"] = counter;
    arr.append(objEvent);

    obj["events"] = arr;
    setData(obj);
}

bool TakePutResultData::isEnabledTake() const
{
    auto permis = data()["permissions"].toObject();
    return permis["take"].toBool(true);
}

void TakePutResultData::setIsEnabledTake(const bool isEnabled)
{
    QJsonObject obj = data();
    auto permis = obj["permissions"].toObject();
    permis["take"] = isEnabled;
    obj["permissions"] = permis;
    setData(obj);
}

bool TakePutResultData::isEnabledPut() const
{
    auto permis = data()["permissions"].toObject();
    return permis["put"].toBool(true);
}

void TakePutResultData::setIsEnabledPut(const bool isEnabled)
{
    QJsonObject obj = data();
    auto permis = obj["permissions"].toObject();
    permis["put"] = isEnabled;
    obj["permissions"] = permis;
    setData(obj);
}

bool TakePutResultData::isEnabledErrors() const
{
    auto permis = data()["permissions"].toObject();
    return permis["errors"].toBool(true);
}

void TakePutResultData::setIsEnabledErrors(const bool isEnabled)
{
    QJsonObject obj = data();
    auto permis = obj["permissions"].toObject();
    permis["errors"] = isEnabled;
    obj["permissions"] = permis;
    setData(obj);
}
