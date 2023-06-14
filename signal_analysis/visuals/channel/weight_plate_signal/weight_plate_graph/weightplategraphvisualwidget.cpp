#include "weightplategraphvisualwidget.h"
#include "ui_weightplategraphvisualwidget.h"

#include <QMessageBox>
#include <QDebug>

#include "channelsdefines.h"
#include "channelsutils.h"
#include "baseutils.h"
#include "dataprovider.h"
#include "weightplatesignal.h"
#include "createsectiondialog.h"
#include "signalanalysisutils.h"

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

    ui->wgtGraph->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->wgtGraph, &AreaGraph::customContextMenuRequested, this, &WeightPlateGraphVisualWidget::on_popupMenuRequested);

    connect(ui->wgtGraph, &AreaGraph::press, this, &WeightPlateGraphVisualWidget::on_press);
    connect(ui->wgtGraph, &AreaGraph::move, this, &WeightPlateGraphVisualWidget::on_move);
    connect(ui->wgtGraph, &AreaGraph::mouseRelease, this, &WeightPlateGraphVisualWidget::on_release);
}

WeightPlateGraphVisualWidget::~WeightPlateGraphVisualWidget()
{
    delete ui;
}

bool WeightPlateGraphVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctWeightPlate;
}

void WeightPlateGraphVisualWidget::calculate()
{
    getSignal();
    showGraph(-1);
    m_isCalculate = true;
}

void WeightPlateGraphVisualWidget::scaleChange(int idx)
{
    if (m_isCalculate)
    {
        int v = scaleMultiplier(idx);
        for (int i = 0; i < ui->wgtGraph->areasesCount(); ++i)
        {
            auto min = m_signal->minValueChan(i);
            auto max = m_signal->maxValueChan(i);
            double diap = (max - min) / v * 1.2;

            ui->wgtGraph->setDiapazone(i, (max + min) / 2 - diap / 2, (max + min) / 2 + diap / 2);
        }
        setDiapazones();
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

    setDiapazones();
}

void WeightPlateGraphVisualWidget::btnPlusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
    setDiapazones();
}

void WeightPlateGraphVisualWidget::btnMinusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 0.0625) //1)
        ui->wgtGraph->setHScale(hScale / 2);
    setDiapazones();
}

void WeightPlateGraphVisualWidget::signalScroll(int pos)
{
    ui->wgtGraph->setStartPoint(ui->wgtGraph->area(0)->signal()->size() * pos / 100);
    setDiapazones();
}

void WeightPlateGraphVisualWidget::on_selectChannel(int chanIdx)
{
    if (m_isCalculate)
    {
        showGraph(chanIdx - 1);
        setDiapazones();
    }
}

void WeightPlateGraphVisualWidget::on_popupMenuRequested(QPoint pos)
{
    if (!m_menu)
    {
        m_menu = new QMenu(this);
        QAction * createSection = new QAction(tr("Создать секцию"), this);
        connect(createSection, &QAction::triggered, this, &WeightPlateGraphVisualWidget::on_createSection);
        m_menu->addAction(createSection);
    }
    m_menu->popup(ui->wgtGraph->mapToGlobal(pos));
}

void WeightPlateGraphVisualWidget::on_createSection()
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
            QMessageBox::information(nullptr, tr("Сообщение"), tr("Не выделен участок сигнала"));
        else
            QMessageBox::information(nullptr, tr("Сообщение"), tr("Неправильно выделен участок сигнала"));
    }
}

void WeightPlateGraphVisualWidget::on_press(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        ui->wgtGraph->clearSelectArea();
        m_selBeg = x;
    }
}

void WeightPlateGraphVisualWidget::on_release(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        m_selBeg = -1;
    }
}

void WeightPlateGraphVisualWidget::on_move(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        if (m_selBeg > -1)
            ui->wgtGraph->selectArea(m_selBeg, x);
    }
}

int WeightPlateGraphVisualWidget::scaleMultiplier(const int scaleIdx) const
{
    int v = 1;
    for (int i = 0; i < scaleIdx; ++i)
        v *= 2;
    return v;
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
    if (DataProvider::getChannel(probeUid(), channelId(), data))
    {
        if (!m_signal)
        {
            m_signal = new WeightPlateSignal(data);
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                ui->cbChannels->addItem(m_signal->subChanName(i));
        }
    }
}

void WeightPlateGraphVisualWidget::showGraph(const int chanIdx)
{
    auto show = [&](const int chan, const int chanDiap)
    {
        ui->wgtGraph->appendSignal(m_signal, m_signal->subChanName(chan), chan);
        auto min = m_signal->minValueChan(chan);
        auto max = m_signal->maxValueChan(chan);
        ui->wgtGraph->setDiapazone(chanDiap, min  - (max - min) * 0.1, max + (max - min) * 0.1);
    };

    if (m_signal)
    {
        ui->wgtGraph->clear();
        m_selectedChan = chanIdx;
        if (chanIdx == -1)
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                show(i, i);
        else
            show(chanIdx, 0);
    }
}

void WeightPlateGraphVisualWidget::setDiapazones()
{
    if (m_isCalculate)
    {
            QList<double> mids;
            mids.clear();
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                mids << 0;

            int n = 0;
            for (int i = ui->wgtGraph->startPoint(); i < ui->wgtGraph->startPoint() + ui->wgtGraph->areaWidth() / ui->wgtGraph->hScale(); ++i)
            {
                if (i < m_signal->size())
                {
                    for (int j = 0; j < m_signal->subChansCount(); ++j)
                    {
                        auto v = m_signal->value(j, i);
                        auto mid = mids.at(j);
                        mid += v;
                        mids.replace(j, mid);
                    }
                    ++n;
                }
            }
            if (n > 0)
                for (int i = 0; i < m_signal->subChansCount(); ++i)
                {
                    auto mid = mids.at(i);
                    mid /= n;
                    mids.replace(i, mid);
                }

            int v = scaleMultiplier(ui->cbScale->currentIndex());
            if (ui->wgtGraph->areasesCount() == m_signal->subChansCount())
            {
                for (int i = 0; i < m_signal->subChansCount(); ++i)
                {
                    auto min = m_signal->minValueChan(i);
                    auto max = m_signal->maxValueChan(i);
                    double diap = (max - min) / v * 1.2;
                    ui->wgtGraph->setDiapazone(i, mids.at(i) - diap / 2, mids.at(i) + diap / 2);
                }
            }
            else
            {
                if (m_selectedChan >= 0 && m_selectedChan < m_signal->subChansCount())
                {
                    auto min = m_signal->minValueChan(m_selectedChan);
                    auto max = m_signal->maxValueChan(m_selectedChan);
                    double diap = (max - min) / v * 1.2;
                    ui->wgtGraph->setDiapazone(0,
                                               mids.at(m_selectedChan) - diap / 2,
                                               mids.at(m_selectedChan) + diap / 2);
                }
            }
    }
}
