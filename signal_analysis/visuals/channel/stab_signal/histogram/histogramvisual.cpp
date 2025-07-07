#include "histogramvisual.h"

#include <QCoreApplication>

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
    return QCoreApplication::tr("Гистограмма"); //VisualNames::Name_Histogram;
}

Visual *HistogramVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelId,
                                         const QString& sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new HistogramVisualWidget(this, testUid, probeUid, channelId, parent);
}
