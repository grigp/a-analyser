#include "variationpulsogramvisualwidget.h"
#include "ui_variationpulsogramvisualwidget.h"

#include "channelsdefines.h"
#include "channelsutils.h"

VariationPulsogramVisualWidget::VariationPulsogramVisualWidget(VisualDescriptor* visual,
                                                               const QString& testUid,
                                                               const QString& probeUid,
                                                               const QString& channelId,
                                                               QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelId, parent),
    ui(new Ui::VariationPulsogramVisualWidget)
{
    ui->setupUi(this);
}

VariationPulsogramVisualWidget::~VariationPulsogramVisualWidget()
{
    delete ui;
}

bool VariationPulsogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctRitmogram;
}

void VariationPulsogramVisualWidget::calculate()
{

}
