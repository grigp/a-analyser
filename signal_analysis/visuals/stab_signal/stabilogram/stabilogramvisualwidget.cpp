#include "stabilogramvisualwidget.h"
#include "ui_stabilogramvisualwidget.h"

#include <QDebug>

#include "visualdescriptor.h"
#include "channelsutils.h"
#include "channelsdefines.h"

StabilogramVisualWidget::StabilogramVisualWidget(VisualDescriptor* visual,
                                                 const QString& testUid, const QString& probeUid, const QString& channelUid,
                                                 QWidget *parent) :
    ChannelVisual(visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::StabilogramVisualWidget)
{
    ui->setupUi(this);
}

StabilogramVisualWidget::~StabilogramVisualWidget()
{
    delete ui;
}

bool StabilogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctStabilogram;
}

void StabilogramVisualWidget::calculate()
{

}
