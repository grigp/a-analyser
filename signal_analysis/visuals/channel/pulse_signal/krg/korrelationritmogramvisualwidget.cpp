#include "korrelationritmogramvisualwidget.h"
#include "ui_korrelationritmogramvisualwidget.h"

#include <QDebug>

#include "aanalyserapplication.h"
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
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::channelChanged,
            this, &KorrelationRitmogramVisualWidget::on_channelChanged);
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
    showKRG();

    ui->wgtKRG->setNameHorizontal(tr("ЧСС (i), уд/мин"));
    ui->wgtKRG->setNameVertical(tr("ЧСС (i-1), уд/мин"));

}

void KorrelationRitmogramVisualWidget::on_splitterMoved(int, int)
{
    saveSplitterPosition();
}

void KorrelationRitmogramVisualWidget::on_channelChanged(const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(probeUid);
    Q_UNUSED(channelId);
    if (m_signal)
    {
        delete m_signal;
        m_signal = nullptr;
    }
    showKRG();
}

void KorrelationRitmogramVisualWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("Geometry/KRGVisualWidget", "VSplitterPosition", ui->splitter->saveState());

}

void KorrelationRitmogramVisualWidget::restoreSplitterPosition()
{
    ui->splitter->setSizes(QList<int>() << geometry().width() / 4 * 3 << geometry().width() / 4);

    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/KRGVisualWidget", "VSplitterPosition").toByteArray();
    ui->splitter->restoreState(val);
}

void KorrelationRitmogramVisualWidget::showKRG()
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
}
