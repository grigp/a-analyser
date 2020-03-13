#include "dynamosignal.h"

DynamoSignal::DynamoSignal(const QString &chanId, const int freq)
    : SingleSignal(chanId, freq)
{

}

DynamoSignal::DynamoSignal(const QByteArray &data)
    : SingleSignal(data)
{

}
