#include "wpgraphanalysisvisual.h"

#include "wpgraphanalysisvisualwidget.h"

WPGraphAnalysisVisual::WPGraphAnalysisVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

WPGraphAnalysisVisual::~WPGraphAnalysisVisual()
{

}

QString WPGraphAnalysisVisual::uid()
{
    return VisualNames::UID_WPGraphAnalysis;
}

QString WPGraphAnalysisVisual::name()
{
    return VisualNames::Name_WPGraphAnalysis;
}

Visual *WPGraphAnalysisVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new WPGraphAnalysisVisualWidget(this, testUid, probeUid, channelUid, parent);
}
