#include "dynamosignalvisualwidget.h"
#include "ui_dynamosignalvisualwidget.h"

#include "baseutils.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "dynamosignal.h"

DynamoSignalVisualWidget::DynamoSignalVisualWidget(VisualDescriptor* visual,
                                                   const QString& testUid, const QString& probeUid, const QString& channelUid,
                                                   QWidget *parent)
    : ChannelVisual(visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::DynamoSignalVisualWidget)
{
    ui->setupUi(this);

    foreach (auto scaleName, BaseUtils::ScalesDyn)
        ui->cbScale->addItem(scaleName, BaseUtils::ScaleKoefitients.value(scaleName));
    ui->sbSignal->setEnabled(false);
    ui->wgtGraph->setIsShowCursor(true);

    QCursor cursorGraph = QCursor(QPixmap(":/images/SignalCursor.png"));
    ui->wgtGraph->setCursor(cursorGraph);

    connect(ui->wgtGraph, &AreaGraph::moveCursor, this, &DynamoSignalVisualWidget::on_moveCursor);
}

DynamoSignalVisualWidget::~DynamoSignalVisualWidget()
{
    delete ui;
}

bool DynamoSignalVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctDynamo ||
           ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctBreath;
}

void DynamoSignalVisualWidget::calculate()
{
    showGraph();
}

void DynamoSignalVisualWidget::scaleChange(int idx)
{
    Q_UNUSED(idx);
    double diap = BaseUtils::DynDefaultDiap / ui->cbScale->currentData().toDouble();
    if (m_signal)
    {
        if (ui->cbScale->currentData().toDouble() > 1)
        {
            if (ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctBreath)
                ui->wgtGraph->setDiapazone(m_signal->midValue() - diap / 2, m_signal->midValue() + diap / 2);
            else
            if (ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctDynamo)
                ui->wgtGraph->setDiapazone(0, diap);
        }
        else
            ui->wgtGraph->setDiapazone(0, diap);
    }
}

void DynamoSignalVisualWidget::btnFulSignalClicked(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    else
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_scrolling);
}

void DynamoSignalVisualWidget::btnPlusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
}

void DynamoSignalVisualWidget::btnMinusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 1)
        ui->wgtGraph->setHScale(hScale / 2);
}

void DynamoSignalVisualWidget::signalScroll(int pos)
{
    int w = ui->wgtGraph->width() - ui->wgtGraph->leftSpace() - ui->wgtGraph->rightSpace();
    if (ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() > w)
    {
        int p = static_cast<int>((ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() - w)
                                 * pos / 100.0 / ui->wgtGraph->hScale());
        ui->wgtGraph->setStartPoint(p);
    }
}

void DynamoSignalVisualWidget::on_moveCursor()
{
    ui->edCursor->setText(QString::number(static_cast<double>(ui->wgtGraph->cursorPos()) / static_cast<double>(m_signal->frequency())));
    auto vals = ui->wgtGraph->cursorValues();
    if (vals.size() == 1)
    {
        ui->edValue->setText(QString::number(vals.at(0)));
    }
}

void DynamoSignalVisualWidget::showGraph()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelUid(), data))
    {
        if (!m_signal)
        {
            m_signal = new DynamoSignal(data);
            QString title = "";
            if (ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctDynamo)
                title = tr("кг");
            ui->wgtGraph->appendSignal(m_signal, title);
            auto absMax = m_signal->absMaxValue();
            double v = 1;
//            while (v < absMax)
//                v *= 2;

            v = BaseUtils::DynDefaultDiap;
            if (absMax > BaseUtils::DynDefaultDiap)
                while (v < absMax)
                    v *= 2;
            ui->wgtGraph->setDiapazone(0, v);

            //! Позиция в переключателе масштаба
            double scM = BaseUtils::DynDefaultDiap / static_cast<double>(v);
            for (int i = 0; i < ui->cbScale->count(); ++i)
                if (static_cast<int>(ui->cbScale->itemData(i).toDouble() * 10000) == static_cast<int>(scM * 10000)) // * 10000 - уход от double
                {
                    ui->cbScale->setCurrentIndex(i);
                    break;
                }
        }
    }
}
