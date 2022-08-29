#include "filterlowfrequency.h"

#include "baseutils.h"
#include "signaltransformerdefines.h"
#include "filterlowfrequencyparamswidget.h"

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

void FilterLowFrequency::transform(QVector<double> &buffer, const QJsonObject& params)
{
   auto freq = params["freq"].toDouble(1.0);

   BaseUtils::filterLowFreq(buffer, 50, freq, BaseUtils::fkChebyshev, 0, buffer.size() - 1);
   BaseUtils::swapVector(buffer);
   BaseUtils::filterLowFreq(buffer, 50, freq, BaseUtils::fkChebyshev, 0, buffer.size() - 1);
   BaseUtils::swapVector(buffer);

}

SignalTransformerParamsWidget *FilterLowFrequency::getParamsWidget()
{
    return new FilterLowFrequencyParamsWidget(nullptr);
}
