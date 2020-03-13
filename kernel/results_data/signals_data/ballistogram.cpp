#include "ballistogram.h"


Ballistogram::Ballistogram(const QString &chanId, const int freq)
    : SingleSignal(chanId, freq)
{

}

Ballistogram::Ballistogram(const QByteArray &data)
    : SingleSignal(data)
{

}
