#include "ritmogram.h"

Ritmogram::Ritmogram(const QString &chanId, const int freq)
    : SingleSignal(chanId, freq)
{

}

Ritmogram::Ritmogram(const QByteArray &data)
    : SingleSignal(data)
{

}

void Ritmogram::removeValue(const int rec)
{
    doRemoveValue(rec);
}
