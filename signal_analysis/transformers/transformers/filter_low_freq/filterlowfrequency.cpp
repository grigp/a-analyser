#include "filterlowfrequency.h"

#include <QDebug>

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

   auto freqCutoff = params["freq_cutoff"].toDouble(1.0);
   auto freqSample = params["freq_sample"].toDouble(100);

   auto kind = static_cast<BaseUtils::FilterKind>(params["kind"].toInt());

   BaseUtils::filterLowFreq(buffer, freqSample, freqCutoff, kind, 0, buffer.size() - 1);
   BaseUtils::swapVector(buffer);
   BaseUtils::filterLowFreq(buffer, freqSample, freqCutoff, kind, 0, buffer.size() - 1);
   BaseUtils::swapVector(buffer);

}

SignalTransformerParamsWidget *FilterLowFrequency::getParamsWidget()
{
    return new FilterLowFrequencyParamsWidget(nullptr);
}
