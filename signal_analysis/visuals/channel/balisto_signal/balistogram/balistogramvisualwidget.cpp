#include "balistogramvisualwidget.h"
#include "ui_balistogramvisualwidget.h"

#include <QMessageBox>
#include <QDebug>

#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "balistogram.h"
#include "baseutils.h"
#include "createsectiondialog.h"
#include "signalanalysisutils.h"

BalistogramVisualWidget::BalistogramVisualWidget(VisualDescriptor *visual,
                                                 const QString &testUid, const QString &probeUid, const QString &channelId,
                                                 QWidget *parent)
    : ChannelVisual(visual, testUid, probeUid, channelId, parent),
      ui(new Ui::BalistogramVisualWidget)
{
    ui->setupUi(this);

    foreach (auto scaleName, BaseUtils::ScalesZ)
        ui->cbScale->addItem(scaleName, BaseUtils::ScaleKoefitients.value(scaleName));
    ui->sbSignal->setEnabled(false);
    ui->wgtGraph->setIsShowCursor(true);

    QCursor cursorGraph = QCursor(QPixmap(":/images/SignalCursor.png"));
    ui->wgtGraph->setCursor(cursorGraph);

    ui->wgtGraph->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->wgtGraph, &AreaGraph::customContextMenuRequested, this, &BalistogramVisualWidget::on_popupMenuRequested);

    connect(ui->wgtGraph, &AreaGraph::moveCursor, this, &BalistogramVisualWidget::on_moveCursor);
    connect(ui->wgtGraph, &AreaGraph::press, this, &BalistogramVisualWidget::on_press);
    connect(ui->wgtGraph, &AreaGraph::move, this, &BalistogramVisualWidget::on_move);
    connect(ui->wgtGraph, &AreaGraph::mouseRelease, this, &BalistogramVisualWidget::on_release);
}

BalistogramVisualWidget::~BalistogramVisualWidget()
{
    delete ui;
}

bool BalistogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctBalistogram;
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

void BalistogramVisualWidget::on_popupMenuRequested(QPoint pos)
{
    if (!m_menu)
    {
        m_menu = new QMenu(this);
        QAction * createSection = new QAction(tr("Создать секцию"), this);
        connect(createSection, &QAction::triggered, this, &BalistogramVisualWidget::on_createSection);
        m_menu->addAction(createSection);
    }
    m_menu->popup(ui->wgtGraph->mapToGlobal(pos));
}

void BalistogramVisualWidget::on_createSection()
{
    int begin = -1;
    int end = -1;
    ui->wgtGraph->selectedArea(begin, end);
    if (begin < end && end > 0)
    {
        CreateSectionDialog dlg;
        dlg.assignSignal(m_z);
        if (dlg.exec() == QDialog::Accepted)
        {
            QString chId = m_z->channelId();
            QString chUid = DataProvider::getChannelUid(probeUid(), channelId());
            QString name = dlg.sectionName();
            SignalAnalysisUtils::createSection(chUid, chId, name, dlg.channel(), begin, end, m_z);
        }
    }
    else
    {
        if (begin == -1 && end == -1)
            QMessageBox::information(nullptr, tr("Сообщение"), tr("Не выделен участок сигнала"));
        else
            QMessageBox::information(nullptr, tr("Сообщение"), tr("Неправильно выделен участок сигнала"));
    }
}

void BalistogramVisualWidget::on_press(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        ui->wgtGraph->clearSelectArea();
        m_selBeg = x;
    }
}

void BalistogramVisualWidget::on_release(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        m_selBeg = -1;
    }
}

void BalistogramVisualWidget::on_move(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        if (m_selBeg > -1)
            ui->wgtGraph->selectArea(m_selBeg, x);
    }
}

void BalistogramVisualWidget::showGraph()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelId(), data))
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
