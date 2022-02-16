#include "vectoranalysisvisualwidget.h"
#include "ui_vectoranalysisvisualwidget.h"

#include "channelsdefines.h"
#include "channelsutils.h"

VectorAnalysisVisualWidget::VectorAnalysisVisualWidget(VisualDescriptor* visual,
                                                       const QString& testUid, const QString& probeUid, const QString& channelUid,
                                                       QWidget *parent) :
    ChannelVisual(visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::VectorAnalysisVisualWidget)
{
    ui->setupUi(this);
}

VectorAnalysisVisualWidget::~VectorAnalysisVisualWidget()
{
    delete ui;
}

bool VectorAnalysisVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctStabilogram;
}

void VectorAnalysisVisualWidget::calculate()
{

}
