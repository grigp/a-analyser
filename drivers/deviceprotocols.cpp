#include "deviceprotocols.h"


QString DeviceProtocols::portName(const DeviceProtocols::ports port)
{
    if (port == pcEmulation)
        return "Эмуляция";
    else
    if (port == pcUsb)
        return "USB";
    else
        return QString("COM %1").arg(static_cast<int>(port));
}
