#include "vectoranalysisvisual.h"

#include "vectoranalysisvisualwidget.h"

VectorAnalysisVisual::VectorAnalysisVisual(BaseDefines::TestLevel level)
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
                                              const QString &testUid, const QString &probeUid, const QString &channelId,
                                              const QString& sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new VectorAnalysisVisualWidget(this, testUid, probeUid, channelId, parent);
}
