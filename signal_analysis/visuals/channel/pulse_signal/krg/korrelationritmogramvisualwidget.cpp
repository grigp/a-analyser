#include "korrelationritmogramvisualwidget.h"
#include "ui_korrelationritmogramvisualwidget.h"

#include "dataprovider.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "settingsprovider.h"
#include "ritmogram.h"

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
    if (m_signal)
        delete m_signal;
    delete ui;
}

bool KorrelationRitmogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctRitmogram;
}

void KorrelationRitmogramVisualWidget::calculate()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelId(), data))
    {
        if (!m_signal)
        {
            //! Назначить сигнал
            m_signal = new Ritmogram(data);

            if (m_signal->size() > 0)
            {
                ui->wgtKRG->appendSignal(m_signal);
            }
        }
    }

    ui->wgtKRG->setNameHorizontal(tr("ЧСС (i), уд/мин"));
    ui->wgtKRG->setNameVertical(tr("ЧСС (i-1), уд/мин"));
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
