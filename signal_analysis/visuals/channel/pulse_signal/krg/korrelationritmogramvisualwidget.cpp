#include "korrelationritmogramvisualwidget.h"
#include "ui_korrelationritmogramvisualwidget.h"

#include "channelsdefines.h"
#include "channelsutils.h"
#include "settingsprovider.h"


KorrelationRitmogramVisualWidget::KorrelationRitmogramVisualWidget(VisualDescriptor* visual,
                                                                   const QString& testUid, const QString& probeUid, const QString& channelId,
                                                                   QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelId, parent),
    ui(new Ui::KorrelationRitmogramVisualWidget)
{
    ui->setupUi(this);
    restoreSplitterPosition();
}

KorrelationRitmogramVisualWidget::~KorrelationRitmogramVisualWidget()
{
    delete ui;
}

bool KorrelationRitmogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctRitmogram;
}

void KorrelationRitmogramVisualWidget::calculate()
{

}

void KorrelationRitmogramVisualWidget::on_splitterMoved(int, int)
{
    saveSplitterPosition();
}

void KorrelationRitmogramVisualWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("KRGVisualWidget", "VSplitterPosition", ui->splitter->saveState());

}

void KorrelationRitmogramVisualWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("KRGVisualWidget", "VSplitterPosition").toByteArray();
    ui->splitter->restoreState(val);
}
