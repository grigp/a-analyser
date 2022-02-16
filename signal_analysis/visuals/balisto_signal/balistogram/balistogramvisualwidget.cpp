#include "balistogramvisualwidget.h"
#include "ui_balistogramvisualwidget.h"

#include "channelsdefines.h"
#include "channelsutils.h"

BalistogramVisualWidget::BalistogramVisualWidget(VisualDescriptor *visual,
                                                 const QString &testUid, const QString &probeUid, const QString &channelUid,
                                                 QWidget *parent)
    : ChannelVisual(visual, testUid, probeUid, channelUid, parent),
      ui(new Ui::BalistogramVisualWidget)
{
    ui->setupUi(this);

}

BalistogramVisualWidget::~BalistogramVisualWidget()
{
    delete ui;
}

bool BalistogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctBallistogram;
}

void BalistogramVisualWidget::calculate()
{

}
