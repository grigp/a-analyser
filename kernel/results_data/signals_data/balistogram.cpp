#include "balistogram.h"


Balistogram::Balistogram(const QString &chanId, const int freq)
    : SingleSignal(chanId, freq)
{

}

Balistogram::Balistogram(const QByteArray &data)
    : SingleSignal(data)
{

}
