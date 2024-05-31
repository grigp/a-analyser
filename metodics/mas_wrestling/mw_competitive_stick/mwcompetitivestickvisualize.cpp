#include "mwcompetitivestickvisualize.h"
#include "ui_mwcompetitivestickvisualize.h"

#include <QDebug>

#include "baseutils.h"
#include "channelsutils.h"
#include "mwcompetitivestickcalculator.h"
#include "testresultdata.h"
#include "resultinfo.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "resultinfo.h"
#include "channelsdefines.h"

MWCompetitiveStickVisualize::MWCompetitiveStickVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MWCompetitiveStickVisualize)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    ui->wgtGraph->setIsShowCursor(true);

    QCursor cursorGraph = QCursor(QPixmap(":/images/SignalCursor.png"));
    ui->wgtGraph->setCursor(cursorGraph);

    connect(ui->wgtGraph, &AreaGraph::moveCursor, this, &MWCompetitiveStickVisualize::on_moveCursor);
    foreach (auto scaleName, BaseUtils::ScalesDyn)
        ui->cbScale->addItem(scaleName, BaseUtils::ScaleKoefitients.value(scaleName));
}

MWCompetitiveStickVisualize::~MWCompetitiveStickVisualize()
{
    if (m_trd)
        delete m_trd;
    delete ui;
}

void MWCompetitiveStickVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new MWCompetitiveStickCalculator(testUid, this);
        m_calculator->calculate();

        ui->lblMaxForce->setText(tr("Максимальное усилие") + " - " + m_calculator->maxForceFmt() + " " + tr("кг"));
        ui->lblAverageForce->setText(tr("Среднее усилие") + " - " + m_calculator->averageForceFmt() + " " + tr("кг"));

        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            m_absMax = 0;
            m_trd->openTest(testUid);
            Q_ASSERT(ti.probes.size() == m_trd->probesCount());
            for (int i = 0; i < ti.probes.size(); ++i)
            {
                DataDefines::ProbeInfo pi;
                if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
                {
                    auto* probe = m_trd->probe(i);

                    m_signal = probe->signal(ChannelsDefines::chanMWStickForce);
                    if (m_signal && m_signal->size() > 0)
                    {
                        auto max = m_signal->absMaxValue();
                        if (max > m_absMax)
                            m_absMax = max;

                        //! Графики сигналов
                        ui->wgtGraph->appendSignal(m_signal, pi.name);
                        ui->wgtGraph->setLegend(i, QStringList() << "Усилие");
                        ui->wgtGraph->setDiapazone(i, 0, m_absMax);

                        //! Позиция в переключателе масштаба
                        double scM = BaseUtils::MWStickDefaultDiap / static_cast<double>(m_absMax);
                        for (int i = ui->cbScale->count() - 1; i >= 0 ; --i)
                            if (static_cast<int>(ui->cbScale->itemData(i).toDouble() * 10000) <= static_cast<int>(scM * 10000)) // * 10000 - уход от double
                            {
                                ui->cbScale->setCurrentIndex(i);
                                break;
                            }
                    }
                }
            }
        }
    }
}

void MWCompetitiveStickVisualize::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

void MWCompetitiveStickVisualize::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, MWCompetitiveStickCalculator *calculator)
{
    Q_UNUSED(painter);
    Q_UNUSED(testUid);
    Q_UNUSED(rect);
    Q_UNUSED(calculator);
}

void MWCompetitiveStickVisualize::on_scaleChange(int idx)
{
    Q_UNUSED(idx);
    double diap = BaseUtils::MWStickDefaultDiap / ui->cbScale->currentData().toDouble();
        ui->wgtGraph->setDiapazone(0, diap);
}

void MWCompetitiveStickVisualize::btnFulSignalClicked(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    else
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_scrolling);
}

void MWCompetitiveStickVisualize::btnPlusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
}

void MWCompetitiveStickVisualize::btnMinusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 1)
        ui->wgtGraph->setHScale(hScale / 2);
}

void MWCompetitiveStickVisualize::on_signalScroll(int pos)
{
    int w = ui->wgtGraph->width() - ui->wgtGraph->leftSpace() - ui->wgtGraph->rightSpace();
    if (ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() > w)
    {
        int p = static_cast<int>((ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() - w)
                                 * pos / 100.0 / ui->wgtGraph->hScale());
        ui->wgtGraph->setStartPoint(p);
    }
}

void MWCompetitiveStickVisualize::on_moveCursor()
{
    ui->edCursor->setText(QString::number(static_cast<double>(ui->wgtGraph->cursorPos()) / static_cast<double>(m_signal->frequency())));
    auto vals = ui->wgtGraph->cursorValues();
    if (vals.size() == 1)
    {
        ui->edValue->setText(QString::number(vals.at(0)));
    }
}
