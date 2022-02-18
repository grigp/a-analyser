#include "balistogramvisualwidget.h"
#include "ui_balistogramvisualwidget.h"

#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "balistogram.h"

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
    showGraph();

}

void BalistogramVisualWidget::showGraph()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelUid(), data))
    {
        if (!m_z)
        {
            m_z = new Balistogram(data);
            ui->wgtGraph->appendSignal(m_z, tr("кг"));
            auto absMax = m_z->absMaxValue();
            int v = 1;
            while (v < absMax)
                v *= 2;
            ui->wgtGraph->setDiapazone(0, v);
        }
    }
}
