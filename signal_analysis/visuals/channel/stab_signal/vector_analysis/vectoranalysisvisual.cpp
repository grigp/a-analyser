#include "vectoranalysisvisual.h"

#include "vectoranalysisvisualwidget.h"

VectorAnalysisVisual::VectorAnalysisVisual(VisualDefines::Level level)
    : VisualDescriptor (level)
{

}

VectorAnalysisVisual::~VectorAnalysisVisual()
{

}

QString VectorAnalysisVisual::uid()
{
    return VisualNames::UID_VectorAnalysis;
}

QString VectorAnalysisVisual::name()
{
    return VisualNames::Name_VectorAnalysis;
}

Visual *VectorAnalysisVisual::getVisualWidget(QWidget *parent,
                                              const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new VectorAnalysisVisualWidget(this, testUid, probeUid, channelUid, parent);
}