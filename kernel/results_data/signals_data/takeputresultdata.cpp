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

int TakePutResultData::size() const
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
