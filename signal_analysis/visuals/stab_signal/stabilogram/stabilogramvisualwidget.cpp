#include "stabilogramvisualwidget.h"
#include "ui_stabilogramvisualwidget.h"

#include <QDebug>

#include "visualdescriptor.h"
#include "channelsutils.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "stabilogram.h"

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
    delete m_stab;
}

bool StabilogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctStabilogram;
}

void StabilogramVisualWidget::calculate()
{
    showGraph();

}

void StabilogramVisualWidget::showGraph()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelUid(), data))
    {
        if (!m_stab)
        {
            m_stab = new Stabilogram(data);
            ui->wgtGraph->appendSignal(m_stab, tr("мм"));
            auto absMax = m_stab->absMaxValue();
            int v = 1;
            while (v < absMax)
                v *= 2;
            ui->wgtGraph->setDiapazone(-v, v);
        }
    }
}
