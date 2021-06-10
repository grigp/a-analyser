#include "crossresultdata.h"

#include <QJsonArray>
#include <QJsonObject>


CrossResultData::CrossResultData(const QString &chanId)
    : ChannelJsonData (chanId)
{

}

CrossResultData::CrossResultData(const QByteArray &data)
{
    fromByteArray(data);
}

int CrossResultData::stageTime() const
{
    return data()["stage_time"].toInt();
}

void CrossResultData::setStageTime(const int time)
{
    QJsonObject obj = data();
    obj["stage_time"] = time;
    setData(obj);
}

int CrossResultData::freq() const
{
    return data()["freq"].toInt();
}

void CrossResultData::setFreq(const int freq)
{
    QJsonObject obj = data();
    obj["freq"] = freq;
    setData(obj);
}

int CrossResultData::diap() const
{
    return data()["diap"].toInt();
}

void CrossResultData::setDiap(const int diap)
{
    QJsonObject obj = data();
    obj["diap"] = diap;
    setData(obj);
}

int CrossResultData::eventsCount() const
{
    auto arr = data()["events"].toArray();
    return arr.size();
}

bool CrossResultData::event(const int idx, int &kind, int &direction, int &counter)
{
    auto arr = data()["events"].toArray();
    if (idx >= 0 && idx < arr.size())
    {
        auto obj = arr[idx].toObject();
        kind = obj["event_kind"].toInt();
        direction = obj["direction"].toInt();
        counter = obj["counter"].toInt();
        return true;
    }
    else
    {
        kind = -1;
        direction = -1;
        counter = -1;
        return false;
    }
}

void CrossResultData::addEvent(const int kind, const int direction, const int counter)
{
    QJsonObject obj = data();
    auto arr = obj["events"].toArray();

    QJsonObject objEvent;
    objEvent["event_kind"] = kind;
    objEvent["direction"] = direction;
    objEvent["counter"] = counter;
    arr.append(objEvent);

    obj["events"] = arr;
    setData(obj);
}
