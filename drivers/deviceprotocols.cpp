#include "deviceprotocols.h"

#include <QCoreApplication>
#include <QDebug>

namespace  {

QList<DeviceProtocols::Ports> portsEnum(const DeviceProtocols::Ports start,
                                        const DeviceProtocols::Ports finish)
{
    QList<DeviceProtocols::Ports> retval;
    for (int portId = static_cast<int>(start); portId <= static_cast<int>(finish); ++portId)
        retval.append(static_cast<DeviceProtocols::Ports>(portId));
    return retval;
}

}

QString DeviceProtocols::portName(const DeviceProtocols::Ports port)
{
    if (port == pcNone)
        return QCoreApplication::tr("");
    else
    if (port == pcUsb)
        return "USB";
    else
        return QString("COM %1").arg(static_cast<int>(port));
}

QString DeviceProtocols::serialPortName(const DeviceProtocols::Ports port)
{
    if (static_cast<int>(port) > 0)
        return QString("COM%1").arg(static_cast<int>(port));
    return QString("");
}

QList<DeviceProtocols::Ports> DeviceProtocols::comPorts()
{
    return portsEnum(pcCom1, pcCom127);
}

QList<DeviceProtocols::Ports> DeviceProtocols::comUsbPorts()
{
    return portsEnum(pcUsb, pcCom127);
}

QList<DeviceProtocols::Ports> DeviceProtocols::allPorts()
{
    return portsEnum(pcNone, pcCom127);
}


DeviceProtocols::DeviceControl::~DeviceControl()
{

}

DeviceProtocols::CommonControl::~CommonControl()
{

}

DeviceProtocols::DecartCoordControl::~DecartCoordControl()
{

}

DeviceProtocols::LinearChanControl::~LinearChanControl()
{

}

DeviceProtocols::StabControl::~StabControl()
{

}

DeviceProtocols::TensoControl::~TensoControl()
{

}

DeviceProtocols::PulseControl::~PulseControl()
{

}

DeviceProtocols::MyoControl::~MyoControl()
{

}

DeviceProtocols::JumpPlateControl::~JumpPlateControl()
{

}

DeviceProtocols::MultiPlatformControl::~MultiPlatformControl()
{

}

DeviceProtocols::DeviceData::~DeviceData()
{

}

DeviceProtocols::MultiData::~MultiData()
{

}

DeviceProtocols::StabDvcData::~StabDvcData()
{

}

DeviceProtocols::TensoDvcData::~TensoDvcData()
{

}

DeviceProtocols::WeightPlateDvcData::~WeightPlateDvcData()
{

}

DeviceProtocols::ADCValuesDvcData::~ADCValuesDvcData()
{

}

DeviceProtocols::PulseDvcData::~PulseDvcData()
{

}

DeviceProtocols::MyogramDvcData::~MyogramDvcData()
{

}

DeviceProtocols::JumpPlateDvcData::~JumpPlateDvcData()
{

}

DeviceProtocols::JumpPlateBlockData::~JumpPlateBlockData()
{

}

DeviceProtocols::SourceDvcData::~SourceDvcData()
{

}
