#include "stabilogramvisualwidget.h"
#include "ui_stabilogramvisualwidget.h"

#include <QDebug>

#include "visualdescriptor.h"
#include "channelsutils.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "stabilogram.h"
#include "baseutils.h"

StabilogramVisualWidget::StabilogramVisualWidget(VisualDescriptor* visual,
                                                 const QString& testUid, const QString& probeUid, const QString& channelUid,
                                                 QWidget *parent) :
    ChannelVisual(visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::StabilogramVisualWidget)
{
    ui->setupUi(this);

    foreach (auto scaleName, BaseUtils::Scales)
        ui->cbScale->addItem(scaleName, BaseUtils::ScaleKoefitients.value(scaleName));
    ui->sbSignal->setEnabled(false);
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

void StabilogramVisualWidget::scaleChange(int idx)
{
    Q_UNUSED(idx);
    ui->wgtGraph->setDiapazone(-BaseUtils::StabDefaultDiap / ui->cbScale->currentData().toDouble(),
                               BaseUtils::StabDefaultDiap / ui->cbScale->currentData().toDouble());

}

void StabilogramVisualWidget::btnFulSignalClicked(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    else
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_scrolling);
}

void StabilogramVisualWidget::btnZeriongClicked(bool isZeroing)
{
    ui->wgtGraph->setIsZeroing(isZeroing);
}

void StabilogramVisualWidget::btnPlusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
}

void StabilogramVisualWidget::btnMinusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 1)
        ui->wgtGraph->setHScale(hScale / 2);
}

void StabilogramVisualWidget::signalScroll(int pos)
{
    ui->wgtGraph->setStartPoint(ui->wgtGraph->area(0)->signal()->size() * pos / 100);
}

void StabilogramVisualWidget::showGraph()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelUid(), data))
    {
        if (!m_stab)
        {
            //! Назначить сигнал
            m_stab = new Stabilogram(data);
            ui->wgtGraph->appendSignal(m_stab, tr("мм"));
            ui->wgtGraph->setLegend(0, QStringList() << "Фронталь" << "Сагитталь");

            //! Диапазон сигнала
            auto absMax = m_stab->absMaxValue();
            int v = 1;
            while (v < absMax)
                v *= 2;
            ui->wgtGraph->setDiapazone(-v, v);

            //! Позиция в переключателе масштаба
            double scM = BaseUtils::StabDefaultDiap / static_cast<double>(v);
            for (int i = 0; i < ui->cbScale->count(); ++i)
                if (static_cast<int>(ui->cbScale->itemData(i).toDouble() * 10000) == static_cast<int>(scM * 10000)) // * 10000 - уход от double
                {
                    ui->cbScale->setCurrentIndex(i);
                    break;
                }
        }
    }
}
