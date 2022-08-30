#include "filterhighfrequency.h"

#include "baseutils.h"
#include "signaltransformerdefines.h"
#include "filterhighfrequencyparamswidget.h"

FilterHighFrequency::FilterHighFrequency()
{

}

QString FilterHighFrequency::uid()
{
    return SignalTransformerDefines::FilterHighFreq_UID;
}

QString FilterHighFrequency::name()
{
    return SignalTransformerDefines::FilterHighFreq_name;
}

void FilterHighFrequency::transform(QVector<double> &buffer, const QJsonObject &params)
{
    auto freqCutoff = params["freq_cutoff"].toDouble(1.0);
    auto freqSample = params["freq_sample"].toDouble(100);

    auto kind = static_cast<BaseUtils::FilterKind>(params["kind"].toInt());

    QVector<double> src = buffer;

    BaseUtils::filterLowFreq(buffer, freqSample, freqCutoff, kind, 0, buffer.size() - 1);
    BaseUtils::swapVector(buffer);
    BaseUtils::filterLowFreq(buffer, freqSample, freqCutoff, kind, 0, buffer.size() - 1);
    BaseUtils::swapVector(buffer);

    for (int i = 0; i < src.size(); ++i)
        buffer[i] = src[i] - buffer[i];

}

SignalTransformerParamsWidget *FilterHighFrequency::getParamsWidget()
{
    return new FilterHighFrequencyParamsWidget(nullptr);
}
