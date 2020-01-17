#include "jumpplatedata.h"

#include <QJsonArray>
#include <QJsonObject>


JumpHeightSingleData::JumpHeightSingleData(const QString &chanId)
    : ChannelJsonData(chanId)
{

}

JumpHeightSingleData::JumpHeightSingleData(const QByteArray &data)
    : ChannelJsonData()
{
    fromByteArray(data);
}

double JumpHeightSingleData::height() const
{
    return data()["jump_height"].toDouble();
}

void JumpHeightSingleData::setHeight(const double height)
{
    QJsonObject obj = data();
    obj["jump_height"] = height;
    setData(obj);
}

HoppingData::HoppingData(const QString &chanId)
    : ChannelJsonData(chanId)
{

}

HoppingData::HoppingData(const QByteArray &data)
    : ChannelJsonData()
{
    fromByteArray(data);
}

double HoppingData::time() const
{
    return data()["state_time"].toDouble();
}

void HoppingData::setTime(const double time)
{
    QJsonObject obj = data();
    obj["state_time"] = time;
    setData(obj);
}

double HoppingData::height() const
{
    return data()["jump_height"].toDouble();
}

void HoppingData::setHeight(const double height)
{
    QJsonObject obj = data();
    obj["jump_height"] = height;
    setData(obj);
}

JumpHeightData::JumpHeightData(const QString &chanId)
    : ChannelJsonData(chanId)
{

}

JumpHeightData::JumpHeightData(const QByteArray &data)
    : ChannelJsonData()
{
    fromByteArray(data);
}

double JumpHeightData::time() const
{
    return data()["time_test"].toDouble();
}

void JumpHeightData::setTime(const double time)
{
    QJsonObject obj = data();
    obj["time_test"] = time;
    setData(obj);
}

int JumpHeightData::jumpsCount() const
{
    auto arr = data()["jumps"].toArray();
    return arr.size();
}

SignalsDefines::JumpRec JumpHeightData::jump(const int idx) const
{
    auto arr = data()["jumps"].toArray();
    for (int i = 0; i < arr.size(); ++i)
        if (i == idx)
        {
            auto obj = arr.at(i).toObject();
            SignalsDefines::JumpRec ji;
            ji.height = obj["height"].toDouble();
            ji.timeContact = obj["time_contact"].toDouble();
            return ji;
        }

    return SignalsDefines::JumpRec();
}

void JumpHeightData::addJump(const double height, const double timeContact)
{
    QJsonObject obj = data();
    auto arr = obj["jumps"].toArray();

    QJsonObject objJump;
    objJump["height"] = height;
    objJump["time_contact"] = timeContact;
    arr.append(objJump);

    obj["jumps"] = arr;
    setData(obj);
}

TeppingTestData::TeppingTestData(const QString &chanId)
    : ChannelJsonData(chanId)
{

}

TeppingTestData::TeppingTestData(const QByteArray &data)
    : ChannelJsonData()
{
    fromByteArray(data);
}

double TeppingTestData::time() const
{
    return data()["time_test"].toDouble();
}

void TeppingTestData::setTime(const double time)
{
    QJsonObject obj = data();
    obj["time_test"] = time;
    setData(obj);
}

int TeppingTestData::stepsCount(const BaseUtils::Side side) const
{
    QString sPar = "left_leg";
    if (side == BaseUtils::Right)
        sPar = "right_leg";

    auto arr = data()[sPar].toArray();
    return arr.size();
}

SignalsDefines::TeppingStepRec TeppingTestData::step(const BaseUtils::Side side, const int idx) const
{
    QString sPar = "left_leg";
    if (side == BaseUtils::Right)
        sPar = "right_leg";

    auto arr = data()[sPar].toArray();
    for (int i = 0; i < arr.size(); ++i)
        if (i == idx)
        {
            auto obj = arr.at(i).toObject();
            SignalsDefines::TeppingStepRec tsr;
            tsr.timeContact = obj["time_contact"].toDouble();
            return tsr;
        }

    return SignalsDefines::TeppingStepRec();
}

void TeppingTestData::addStep(const BaseUtils::Side side, const double timeContact)
{
    QString sPar = "left_leg";
    if (side == BaseUtils::Right)
        sPar = "right_leg";

    QJsonObject obj = data();
    auto arr = obj[sPar].toArray();

    QJsonObject objStep;
    objStep["time_contact"] = timeContact;
    arr.append(objStep);

    obj[sPar] = arr;
    setData(obj);
}
