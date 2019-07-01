#include "stabilan01.h"


Stabilan01::Stabilan01(QObject *parent)
    : Driver(parent)
{

}

void Stabilan01::setParams(const QJsonObject &params)
{

}

bool Stabilan01::editParams(QJsonObject &params)
{

}

void Stabilan01::start()
{

}

void Stabilan01::stop()
{

}

QStringList Stabilan01::getProtocols()
{
    return QStringList() << DeviceProtocols::uid_StabProtocol;
}

QList<DeviceProtocols::Ports> Stabilan01::getPorts()
{
    return DeviceProtocols::comPorts();
}

void Stabilan01::calibrate()
{

}

void Stabilan01::zeroing()
{

}
