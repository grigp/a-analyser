#include "jumpplate.h"

JumpPlate::JumpPlate(QObject *parent)
    : Driver(parent)
{

}

void JumpPlate::setParams(const QJsonObject &params)
{

}

bool JumpPlate::editParams(QJsonObject &params)
{

}

void JumpPlate::start()
{

}

void JumpPlate::stop()
{

}

QStringList JumpPlate::getProtocols()
{
    return QStringList() << DeviceProtocols::uid_JumpPlateProtocol;
}

void JumpPlate::calibrate()
{

}
