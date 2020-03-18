#include "breathsignal.h"

BreathSignal::BreathSignal(const QString &chanId, const int freq)
    : SingleSignal(chanId, freq)
{

}

BreathSignal::BreathSignal(const QByteArray &data)
    : SingleSignal(data)
{

}
