#include "balistogramvisualwidget.h"
#include "ui_balistogramvisualwidget.h"

#include <QDebug>

#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "balistogram.h"
#include "baseutils.h"

BalistogramVisualWidget::BalistogramVisualWidget(VisualDescriptor *visual,
                                                 const QString &testUid, const QString &probeUid, const QString &channelUid,
                                                 QWidget *parent)
    : ChannelVisual(visual, testUid, probeUid, channelUid, parent),
      ui(new Ui::BalistogramVisualWidget)
{
    ui->setupUi(this);

    foreach (auto scaleName, BaseUtils::ScalesZ)
        ui->cbScale->addItem(scaleName, BaseUtils::ScaleKoefitients.value(scaleName));
    ui->sbSignal->setEnabled(false);
    ui->wgtGraph->setIsShowCursor(true);

    QCursor cursorGraph = QCursor(QPixmap(":/images/SignalCursor.png"));
    ui->wgtGraph->setCursor(cursorGraph);

    connect(ui->wgtGraph, &AreaGraph::moveCursor, this, &BalistogramVisualWidget::on_moveCursor);
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

void BalistogramVisualWidget::scaleChange(int idx)
{
    Q_UNUSED(idx);
    double diap = BaseUtils::ZDefaultDiap / ui->cbScale->currentData().toDouble();
    if (m_z)
    {
        if (ui->cbScale->currentData().toDouble() > 1)
            ui->wgtGraph->setDiapazone(m_z->midValue() - diap / 2, m_z->midValue() + diap / 2);
        else
            ui->wgtGraph->setDiapazone(0, diap);
    }
}

void BalistogramVisualWidget::btnFulSignalClicked(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    else
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_scrolling);
}

void BalistogramVisualWidget::btnPlusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
}

void BalistogramVisualWidget::btnMinusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 1)
        ui->wgtGraph->setHScale(hScale / 2);
}

void BalistogramVisualWidget::signalScroll(int pos)
{
    int w = ui->wgtGraph->width() - ui->wgtGraph->leftSpace() - ui->wgtGraph->rightSpace();
    if (ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() > w)
    {
        int p = static_cast<int>((ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() - w)
                                 * pos / 100.0 / ui->wgtGraph->hScale());
        ui->wgtGraph->setStartPoint(p);
    }
}

void BalistogramVisualWidget::on_moveCursor()
{
    ui->edCursor->setText(QString::number(static_cast<double>(ui->wgtGraph->cursorPos()) / static_cast<double>(m_z->frequency())));
    auto vals = ui->wgtGraph->cursorValues();
    if (vals.size() == 1)
    {
        ui->edZ->setText(QString::number(vals.at(0)));
    }
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
            double v = 1;
//            while (v < absMax)
//                v *= 2;

            v = BaseUtils::ZDefaultDiap;
            if (absMax > BaseUtils::ZDefaultDiap)
                while (v < absMax)
                    v *= 2;
            ui->wgtGraph->setDiapazone(0, v);

            //! Позиция в переключателе масштаба
            double scM = BaseUtils::ZDefaultDiap / static_cast<double>(v);
            for (int i = 0; i < ui->cbScale->count(); ++i)
                if (static_cast<int>(ui->cbScale->itemData(i).toDouble() * 10000) == static_cast<int>(scM * 10000)) // * 10000 - уход от double
                {
                    ui->cbScale->setCurrentIndex(i);
                    break;
                }
        }
    }
}
