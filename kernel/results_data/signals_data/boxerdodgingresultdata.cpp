#include "boxerdodgingresultdata.h"

#include <QJsonArray>
#include <QJsonObject>

BoxerDodgingResultData::BoxerDodgingResultData(const QString &chanId)
    : ChannelJsonData (chanId)
{

}

BoxerDodgingResultData::BoxerDodgingResultData(const QByteArray &data)
{
    fromByteArray(data);
}

int BoxerDodgingResultData::deviationThreshold() const
{
    return data()["deviation_threshold"].toInt();
}

void BoxerDodgingResultData::setDeviationThreshold(const int value)
{
    QJsonObject obj = data();
    obj["deviation_threshold"] = value;
    setData(obj);
}

int BoxerDodgingResultData::freq() const
{
    return data()["freq"].toInt();
}

void BoxerDodgingResultData::setFreq(const int freq)
{
    QJsonObject obj = data();
    obj["freq"] = freq;
    setData(obj);
}

int BoxerDodgingResultData::diap() const
{
    return data()["diap"].toInt();
}

void BoxerDodgingResultData::setDiap(const int diap)
{
    QJsonObject obj = data();
    obj["diap"] = diap;
    setData(obj);
}

int BoxerDodgingResultData::stagesCount() const
{
    auto arr = data()["stages"].toArray();
    return arr.size();
}

bool BoxerDodgingResultData::stage(const int idx, int &code, int &counter)
{
    auto arr = data()["stages"].toArray();
    if (idx >= 0 && idx < arr.size())
    {
        auto obj = arr[idx].toObject();
        code = obj["code"].toInt();
        counter = obj["counter"].toInt();
        return true;
    }
    else
    {
        code = -1;
        counter = 0;
        return false;
    }
}

void BoxerDodgingResultData::addStage(const int code, const int counter)
{
    QJsonObject obj = data();
    auto arr = obj["stages"].toArray();

    QJsonObject objEvent;
    objEvent["code"] = code;
    objEvent["counter"] = counter;
    arr.append(objEvent);

    obj["stages"] = arr;
    setData(obj);
}
