#include "jumpplate.h"

#include "jumpplateparamsdialog.h"

JumpPlate::JumpPlate(QObject *parent)
    : Driver(parent)
{

}

void JumpPlate::setParams(const QJsonObject &params)
{

}

bool JumpPlate::editParams(QJsonObject &params)
{
    JumpPlateParamsDialog dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
        return true;
    }
    return false;
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

QList<DeviceProtocols::Ports> JumpPlate::getPorts()
{
    return DeviceProtocols::comUsbPorts();
}

void JumpPlate::calibrate()
{

}
