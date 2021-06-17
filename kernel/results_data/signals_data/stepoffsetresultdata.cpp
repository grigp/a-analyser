#include "stepoffsetresultdata.h"

#include <QJsonArray>
#include <QJsonObject>

StepOffsetResultData::StepOffsetResultData(const QString &chanId)
    : ChannelJsonData (chanId)
{

}

StepOffsetResultData::StepOffsetResultData(const QByteArray &data)
{
    fromByteArray(data);
}

int StepOffsetResultData::stageTime() const
{
    return data()["stage_time"].toInt();
}

void StepOffsetResultData::setStageTime(const int time)
{
    QJsonObject obj = data();
    obj["stage_time"] = time;
    setData(obj);
}

int StepOffsetResultData::freq() const
{
    return data()["freq"].toInt();
}

void StepOffsetResultData::setFreq(const int freq)
{
    QJsonObject obj = data();
    obj["freq"] = freq;
    setData(obj);
}

int StepOffsetResultData::diap() const
{
    return data()["diap"].toInt();
}

void StepOffsetResultData::setDiap(const int diap)
{
    QJsonObject obj = data();
    obj["diap"] = diap;
    setData(obj);
}

int StepOffsetResultData::force() const
{
    return data()["force"].toInt();
}

void StepOffsetResultData::setForce(const int force)
{
    QJsonObject obj = data();
    obj["force"] = force;
    setData(obj);
}

int StepOffsetResultData::stepsCount() const
{
    auto arr = data()["steps"].toArray();
    return arr.size();
}

bool StepOffsetResultData::step(const int idx, int &counterTo, int &counterFrom)
{
    auto arr = data()["steps"].toArray();
    if (idx >= 0 && idx < arr.size())
    {
        auto obj = arr[idx].toObject();
        counterTo = obj["counter_to"].toInt();
        counterFrom = obj["counter_from"].toInt();
        return true;
    }
    else
    {
        counterTo = -1;
        counterFrom = -1;
        return false;
    }
}

void StepOffsetResultData::addStep(const int counterTo, const int counterFrom)
{
    QJsonObject obj = data();
    auto arr = obj["steps"].toArray();

    QJsonObject objEvent;
    objEvent["counter_to"] = counterTo;
    objEvent["counter_from"] = counterFrom;
    arr.append(objEvent);

    obj["steps"] = arr;
    setData(obj);
}

