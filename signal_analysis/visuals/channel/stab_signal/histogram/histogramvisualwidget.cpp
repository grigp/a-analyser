#include "histogramvisualwidget.h"
#include "ui_histogramvisualwidget.h"

#include "channelsdefines.h"
#include "channelsutils.h"

HistogramVisualWidget::HistogramVisualWidget(VisualDescriptor* visual,
                                             const QString& testUid, const QString& probeUid, const QString& channelUid,
                                             QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::HistogramVisualWidget)
{
    ui->setupUi(this);
}

HistogramVisualWidget::~HistogramVisualWidget()
{
    delete ui;
}

bool HistogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctStabilogram;
}

void HistogramVisualWidget::calculate()
{

}
