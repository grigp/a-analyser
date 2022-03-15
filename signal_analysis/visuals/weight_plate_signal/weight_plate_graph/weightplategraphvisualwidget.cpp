#include "weightplategraphvisualwidget.h"
#include "ui_weightplategraphvisualwidget.h"

#include <QDebug>

#include "channelsdefines.h"
#include "channelsutils.h"
#include "baseutils.h"
#include "dataprovider.h"
#include "weightplatesignal.h"

WeightPlateGraphVisualWidget::WeightPlateGraphVisualWidget(VisualDescriptor* visual,
                                                           const QString& testUid, const QString& probeUid, const QString& channelUid,
                                                           QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::WeightPlateGraphVisualWidget)
{
    ui->setupUi(this);

    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64");
    ui->sbSignal->setEnabled(false);

//    ui->wgtGraph->setIsShowCursor(true);

//    QCursor cursorGraph = QCursor(QPixmap(":/images/SignalCursor.png"));
//    ui->wgtGraph->setCursor(cursorGraph);

//    connect(ui->wgtGraph, &AreaGraph::moveCursor, this, &WeightPlateGraphVisualWidget::on_moveCursor);
}

WeightPlateGraphVisualWidget::~WeightPlateGraphVisualWidget()
{
    delete ui;
}

bool WeightPlateGraphVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctWeightPlate;
}

void WeightPlateGraphVisualWidget::calculate()
{
    getSignal();
    showGraph(-1);
}

void WeightPlateGraphVisualWidget::scaleChange(int idx)
{
    int v = 1;
    for (int i = 0; i < idx; ++i)
        v *= 2;
    for (int i = 0; i < ui->wgtGraph->areasesCount(); ++i)
    {
        auto min = m_signal->minValueChan(i);
        auto max = m_signal->maxValueChan(i);
        double diap = (max - min) / v * 1.2;

        ui->wgtGraph->setDiapazone(i, (max + min) / 2 - diap / 2, (max + min) / 2 + diap / 2);
    }
}

void WeightPlateGraphVisualWidget::btnFulSignalClicked(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    else
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_scrolling);
}

void WeightPlateGraphVisualWidget::btnPlusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
}

void WeightPlateGraphVisualWidget::btnMinusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 1)
        ui->wgtGraph->setHScale(hScale / 2);
}

void WeightPlateGraphVisualWidget::signalScroll(int pos)
{
    ui->wgtGraph->setStartPoint(ui->wgtGraph->area(0)->signal()->size() * pos / 100);
}

void WeightPlateGraphVisualWidget::on_selectChannel(int chanIdx)
{
    showGraph(chanIdx - 1);
}

//void WeightPlateGraphVisualWidget::on_moveCursor()
//{
//    ui->edCursor->setText(QString::number(static_cast<double>(ui->wgtGraph->cursorPos()) / static_cast<double>(m_signal->frequency())));
//    auto vals = ui->wgtGraph->cursorValues();
//    qDebug() << vals.size() << vals;
//    if (vals.size() == 1)
//    {
//        ui->edValue->setText(QString::number(vals.at(0)));
//    }
//}

void WeightPlateGraphVisualWidget::getSignal()
{
    ui->cbChannels->clear();
    ui->cbChannels->addItem(tr("Все"));

    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelUid(), data))
    {
        if (!m_signal)
        {
            m_signal = new WeightPlateSignal(data);
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                ui->cbChannels->addItem(tr("Канал") + " " + QString::number(i+1));
        }
    }
}

void WeightPlateGraphVisualWidget::showGraph(const int chanIdx)
{
    auto show = [&](const int chan, const int chanDiap)
    {
        ui->wgtGraph->appendSignal(m_signal, tr("Канал") + " " + QString::number(chan + 1) + ", " + tr("кг"), chan);
        auto min = m_signal->minValueChan(chan);
        auto max = m_signal->maxValueChan(chan);
        ui->wgtGraph->setDiapazone(chanDiap, min  - (max - min) * 0.1, max + (max - min) * 0.1);
    };

    if (m_signal)
    {
        ui->wgtGraph->clear();
        if (chanIdx == -1)
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                show(i, i);
        else
            show(chanIdx, 0);
    }
}
