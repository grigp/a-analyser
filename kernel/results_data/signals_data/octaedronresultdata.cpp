#include "octaedronresultdata.h"

#include <QJsonArray>
#include <QJsonObject>

OctaedronResultData::OctaedronResultData(const QString &chanId)
    : ChannelJsonData (chanId)
{

}

OctaedronResultData::OctaedronResultData(const QByteArray &data)
{
    fromByteArray(data);
}

int OctaedronResultData::stageTime() const
{
    return data()["stage_time"].toInt();
}

void OctaedronResultData::setStageTime(const int time)
{
    QJsonObject obj = data();
    obj["stage_time"] = time;
    setData(obj);
}

QString OctaedronResultData::circeRoundRuleMode() const
{
    return data()["circe_round_rule_mode"].toString();
}

void OctaedronResultData::setCirceRoundRuleMode(const QString crm)
{
    QJsonObject obj = data();
    obj["circe_round_rule_mode"] = crm;
    setData(obj);
}

int OctaedronResultData::radius() const
{
    return data()["radius"].toInt();
}

void OctaedronResultData::setRadius(const int radius)
{
    QJsonObject obj = data();
    obj["radius"] = radius;
    setData(obj);
}

int OctaedronResultData::freq() const
{
    return data()["freq"].toInt();
}

void OctaedronResultData::setFreq(const int freq)
{
    QJsonObject obj = data();
    obj["freq"] = freq;
    setData(obj);
}

int OctaedronResultData::diap() const
{
    return data()["diap"].toInt();
}

void OctaedronResultData::setDiap(const int diap)
{
    QJsonObject obj = data();
    obj["diap"] = diap;
    setData(obj);
}

int OctaedronResultData::stagesCount() const
{
    auto arr = data()["stages"].toArray();
    return arr.size();
}

bool OctaedronResultData::stage(const int idx, int &code, int &counter, double &x, double &y)
{
    auto arr = data()["stages"].toArray();
    if (idx >= 0 && idx < arr.size())
    {
        auto obj = arr[idx].toObject();
        code = obj["code"].toInt();
        counter = obj["counter"].toInt();
        x = obj["x"].toDouble();
        y = obj["y"].toDouble();
        return true;
    }
    else
    {
        code = -1;
        counter = 0;
        x = 0;
        y = 0;
        return false;
    }
}

void OctaedronResultData::addStage(const int code, const int counter, const double x, const double y)
{
    QJsonObject obj = data();
    auto arr = obj["stages"].toArray();

    QJsonObject objEvent;
    objEvent["code"] = code;
    objEvent["counter"] = counter;
    objEvent["x"] = x;
    objEvent["y"] = y;
    arr.append(objEvent);

    obj["stages"] = arr;
    setData(obj);
}
