#include "stabilogram.h"

#include <QDebug>

Stabilogram::Stabilogram(const QString &chanId, const int freq)
    : DecartCoordinatesSignal (chanId, freq)
{

}

Stabilogram::Stabilogram(const QByteArray &data)
    : DecartCoordinatesSignal (data)
{
}

Stabilogram::~Stabilogram()
{
}

