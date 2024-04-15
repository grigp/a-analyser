#include "myogramsignalvisualwidget.h"
#include "ui_myogramsignalvisualwidget.h"

#include <QDebug>

#include "baseutils.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "myogram.h"
#include "dataprovider.h"
#include "createsectiondialog.h"
#include "signalanalysisutils.h"
#include "amessagebox.h"

MyogramSignalVisualWidget::MyogramSignalVisualWidget(VisualDescriptor* visual,
                                                     const QString& testUid, const QString& probeUid, const QString& channelId,
                                                     QWidget *parent)
    : ChannelVisual(visual, testUid, probeUid, channelId, parent),
    ui(new Ui::MyogramSignalVisualWidget)
{
    ui->setupUi(this);

}

MyogramSignalVisualWidget::~MyogramSignalVisualWidget()
{
    delete ui;
}

bool MyogramSignalVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctMyogram;
}

void MyogramSignalVisualWidget::calculate()
{
    foreach (auto scaleName, BaseUtils::ScalesMyo)
        ui->cbScale->addItem(scaleName, BaseUtils::ScaleKoefitients.value(scaleName));
    ui->sbSignal->setEnabled(false);
    ui->wgtGraph->setIsShowCursor(true);

    QCursor cursorGraph = QCursor(QPixmap(":/images/SignalCursor.png"));
    ui->wgtGraph->setCursor(cursorGraph);

    ui->wgtGraph->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->wgtGraph, &AreaGraph::customContextMenuRequested, this, &MyogramSignalVisualWidget::on_popupMenuRequested);

    connect(ui->wgtGraph, &AreaGraph::moveCursor, this, &MyogramSignalVisualWidget::on_moveCursor);
    connect(ui->wgtGraph, &AreaGraph::press, this, &MyogramSignalVisualWidget::on_press);
    connect(ui->wgtGraph, &AreaGraph::move, this, &MyogramSignalVisualWidget::on_move);
    connect(ui->wgtGraph, &AreaGraph::mouseRelease, this, &MyogramSignalVisualWidget::on_release);

    getSignal();
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

void MyogramSignalVisualWidget::on_popupMenuRequested(QPoint pos)
{
    if (!m_menu)
    {
        m_menu = new QMenu(this);
        QAction * createSection = new QAction(tr("Создать секцию"), this);
        connect(createSection, &QAction::triggered, this, &MyogramSignalVisualWidget::on_createSection);
        m_menu->addAction(createSection);
    }
    m_menu->popup(ui->wgtGraph->mapToGlobal(pos));
}

void MyogramSignalVisualWidget::on_createSection()
{
    int begin = -1;
    int end = -1;
    ui->wgtGraph->selectedArea(begin, end);
    if (begin < end && end > 0)
    {
        CreateSectionDialog dlg;
        dlg.assignSignal(m_signal);
        if (dlg.exec() == QDialog::Accepted)
        {
            QString chId = m_signal->channelId();
            QString chUid = DataProvider::getChannelUid(probeUid(), channelId());
            QString name = dlg.sectionName();
            SignalAnalysisUtils::createSection(chUid, chId, name, dlg.channel(), begin, end, m_signal);
        }
    }
    else
    {
        if (begin == -1 && end == -1)
            AMessageBox::information(nullptr, tr("Сообщение"), tr("Не выделен участок сигнала"));
        else
            AMessageBox::information(nullptr, tr("Сообщение"), tr("Неправильно выделен участок сигнала"));
    }
}

void MyogramSignalVisualWidget::on_press(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        ui->wgtGraph->clearSelectArea();
        m_selBeg = x;
    }
}

void MyogramSignalVisualWidget::on_release(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        m_selBeg = -1;
    }
}

void MyogramSignalVisualWidget::on_move(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        if (m_selBeg > -1)
            ui->wgtGraph->selectArea(m_selBeg, x);
    }
}

void MyogramSignalVisualWidget::on_cbChannelsChanged(int idx)
{
    showGraph(idx - 1);
}

void MyogramSignalVisualWidget::getSignal()
{
    ui->cbChannels->clear();
    ui->cbChannels->addItem(tr("Все"));

    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelId(), data))
    {
        if (!m_signal)
        {
            m_signal = new Myogram(data);
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                ui->cbChannels->addItem(tr("Отведение") + " " + QString::number(i+1));
        }
    }
}

void MyogramSignalVisualWidget::showGraph(const int chanIdx)
{
    auto show = [&](const int chan, const int chanDiap)
    {
        ui->wgtGraph->appendSignal(m_signal, tr("Отведение") + " " + QString::number(chan + 1) + ", " + tr("мВ"), chan);
        ui->wgtGraph->setDiapazone(chanDiap, 0, 2);
    };

    if (m_signal)
    {
        ui->wgtGraph->clear();
//        ui->wgtGraph->setLine1Color(QColor(0, 0, 255));
//        ui->wgtGraph->setLine2Color(QColor(0, 0, 255));
//        ui->wgtGraph->setLine3Color(QColor(0, 0, 255));
//        ui->wgtGraph->setLine4Color(QColor(0, 0, 255));
//        ui->wgtGraph->setLine5Color(QColor(0, 0, 255));
//        ui->wgtGraph->setLine6Color(QColor(0, 0, 255));

        if (chanIdx == -1)
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                show(i, i);
        else
            show(chanIdx, 0);

        ui->cbScale->setCurrentIndex(1);
    }
}
