#include "filterlowfrequency.h"

#include "signaltransformerdefines.h"

FilterLowFrequency::FilterLowFrequency()
    : SignalTransformer ()
{

}

QString FilterLowFrequency::uid()
{
    return SignalTransformerDefines::FilterLowFreq_UID;
}

QString FilterLowFrequency::name()
{
    return SignalTransformerDefines::FilterLowFreq_name;
}

void FilterLowFrequency::transform(QVector<double> &buffer)
{

}
