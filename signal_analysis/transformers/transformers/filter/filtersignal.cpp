#include "filtersignal.h"

#include <QDebug>

#include "signaltransformerdefines.h"
#include "filtersignalparamswidget.h"

FilterSignal::FilterSignal()
    : SignalTransformer ()
{

}

QString FilterSignal::uid()
{
    return SignalTransformerDefines::FilterSignal_UID;
}

QString FilterSignal::name()
{
    return QCoreApplication::tr("Фильтрация сигнала"); //SignalTransformerDefines::FilterSignal_name;
}

void FilterSignal::transform(QVector<double> &buffer, const QJsonObject &params)
{
    auto freqCutoff = params["freq_cutoff"].toDouble(1.0);
    auto freqSample = params["freq_sample"].toInt(100);

    auto kind = static_cast<BaseUtils::FilterKind>(params["kind"].toInt());
    auto direction = static_cast<BaseUtils::FilterDirection>(params["direction"].toInt());

    if (direction == BaseUtils::fdLowFreq || direction == BaseUtils::fdHighFreq)  //! ФНЧ и ФВЧ
    {
        filtraton(buffer, direction, freqCutoff, kind, freqSample);
    }
    else
    if (direction == BaseUtils::fdBand)                                           //! Полосовой
    {
        filtraton(buffer, BaseUtils::fdLowFreq, freqCutoff + freqCutoff / 4 * 3, kind, freqSample);
        filtraton(buffer, BaseUtils::fdHighFreq, freqCutoff - freqCutoff / 4 * 3, kind, freqSample);
    }
    else
    if (direction == BaseUtils::fdNotch)                                          //! Режекторный
    {
        QVector<double> src = buffer;
        filtraton(src, BaseUtils::fdLowFreq, freqCutoff - freqCutoff / 4 * 3, kind, freqSample);
        filtraton(buffer, BaseUtils::fdHighFreq, freqCutoff + freqCutoff / 4 * 3, kind, freqSample);
        for (int i = 0; i < src.size(); ++i)
            buffer[i] = src[i] + buffer[i];
    }
}

SignalTransformerParamsWidget *FilterSignal::getParamsWidget()
{
    return new FilterSignalParamsWidget(nullptr);
}

void FilterSignal::filtraton(QVector<double> &buffer, BaseUtils::FilterDirection dir, double freqCutoff, BaseUtils::FilterKind kind, int freqSample)
{
    QVector<double> src = buffer;

    BaseUtils::filterLowFreq(buffer, freqSample, freqCutoff, kind, 0, buffer.size() - 1);
    BaseUtils::swapVector(buffer);
    BaseUtils::filterLowFreq(buffer, freqSample, freqCutoff, kind, 0, buffer.size() - 1);
    BaseUtils::swapVector(buffer);

    if (dir == BaseUtils::fdHighFreq)
    {
        for (int i = 0; i < src.size(); ++i)
            buffer[i] = src[i] - buffer[i];
    }
}
