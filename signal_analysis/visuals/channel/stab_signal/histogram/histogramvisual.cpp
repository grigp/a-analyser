#include "histogramvisual.h"

#include "histogramvisualwidget.h"

HistogramVisual::HistogramVisual(BaseDefines::TestLevel level)
    : VisualDescriptor(level)
{

}

HistogramVisual::~HistogramVisual()
{

}

QString HistogramVisual::uid()
{
    return VisualNames::UID_Histogram;
}

QString HistogramVisual::name()
{
    return VisualNames::Name_Histogram;
}

Visual *HistogramVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return new HistogramVisualWidget(this, testUid, probeUid, channelId, parent);
}
