#include "sourcedata.h"

SourceData::SourceData(const QString &chanId, const int subChansCount, const int freq)
    : Myogram (chanId, subChansCount, freq)
{

}

SourceData::SourceData(const QByteArray &data)
    : Myogram (data)
{

}
