#include "myogramsignalvisualwidget.h"
#include "ui_myogramsignalvisualwidget.h"

#include <QDebug>

#include "baseutils.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "myogram.h"
#include "dataprovider.h"

MyogramSignalVisualWidget::MyogramSignalVisualWidget(VisualDescriptor* visual,
                                                     const QString& testUid, const QString& probeUid, const QString& channelUid,
                                                     QWidget *parent)
    : ChannelVisual(visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::MyogramSignalVisualWidget)
{
    ui->setupUi(this);

    foreach (auto scaleName, BaseUtils::ScalesMyo)
        ui->cbScale->addItem(scaleName, BaseUtils::ScaleKoefitients.value(scaleName));
    ui->sbSignal->setEnabled(false);
    ui->wgtGraph->setIsShowCursor(true);

    QCursor cursorGraph = QCursor(QPixmap(":/images/SignalCursor.png"));
    ui->wgtGraph->setCursor(cursorGraph);

    connect(ui->wgtGraph, &AreaGraph::moveCursor, this, &MyogramSignalVisualWidget::on_moveCursor);
}

MyogramSignalVisualWidget::~MyogramSignalVisualWidget()
{
    delete ui;
}

bool MyogramSignalVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctMyogram;
}

void MyogramSignalVisualWidget::calculate()
{
    showGraph();
}

void MyogramSignalVisualWidget::scaleChange(int idx)
{
    Q_UNUSED(idx);
    double diap = BaseUtils::MyoDefaultDiap / ui->cbScale->currentData().toDouble();
    if (m_signal)
        ui->wgtGraph->setDiapazone(0, diap);
}

void MyogramSignalVisualWidget::btnFulSignalClicked(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    else
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_scrolling);
}

void MyogramSignalVisualWidget::btnPlusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
}

void MyogramSignalVisualWidget::btnMinusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 1)
        ui->wgtGraph->setHScale(hScale / 2);
}

void MyogramSignalVisualWidget::signalScroll(int pos)
{
    int w = ui->wgtGraph->width() - ui->wgtGraph->leftSpace() - ui->wgtGraph->rightSpace();
    if (ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() > w)
    {
        int p = static_cast<int>((ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() - w)
                                 * pos / 100.0 / ui->wgtGraph->hScale());
        ui->wgtGraph->setStartPoint(p);
    }
}

void MyogramSignalVisualWidget::on_moveCursor()
{
    ui->edCursor->setText(QString::number(static_cast<double>(ui->wgtGraph->cursorPos()) / static_cast<double>(m_signal->frequency())));
    auto vals = ui->wgtGraph->cursorValues();

    if (vals.size() >= m_signal->subChansCount())
    {
        QString s = "";
        for (int i = 0; i < m_signal->subChansCount(); ++i)
            s += (QString::number(i + 1) + ": " + QString::number(vals.at(i), 'f', 2) + "  ");
        ui->edValue->setText(s);
    }
}

void MyogramSignalVisualWidget::showGraph()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelUid(), data))
    {
        if (!m_signal)
        {
            m_signal = new Myogram(data);
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                ui->wgtGraph->appendSignal(m_signal, tr("Отведение") + " " + QString::number(i+1), i);
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                ui->wgtGraph->setDiapazone(i, 0, 2);
            ui->cbScale->setCurrentIndex(1);
        }
    }
}
