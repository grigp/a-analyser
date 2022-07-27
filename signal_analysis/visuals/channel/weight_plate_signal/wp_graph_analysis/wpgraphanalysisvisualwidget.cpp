#include "wpgraphanalysisvisualwidget.h"
#include "ui_wpgraphanalysisvisualwidget.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include "baseutils.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "weightplatesignal.h"
#include "dataprovider.h"
#include "decartcoordinatessignal.h"

WPGraphAnalysisVisualWidget::WPGraphAnalysisVisualWidget(VisualDescriptor* visual,
                                                         const QString& testUid, const QString& probeUid, const QString& channelUid,
                                                         QWidget *parent) :
    ChannelVisual(visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::WPGraphAnalysisVisualWidget)
{
    ui->setupUi(this);
    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64");
    ui->sbSignal->setEnabled(false);
}

WPGraphAnalysisVisualWidget::~WPGraphAnalysisVisualWidget()
{
    delete ui;
}

bool WPGraphAnalysisVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctWeightPlate;
}

void WPGraphAnalysisVisualWidget::calculate()
{
    getSignal();
    filtration();
    showGraph();
    m_isCalculate = true;
}

void WPGraphAnalysisVisualWidget::scaleChange(int idx)
{
    if (m_isCalculate)
    {
//        int v = scaleMultiplier(idx);
//        for (int i = 0; i < ui->wgtAreaZ->areasesCount(); ++i)
//        {
//            auto min = m_signal->minValueChan(i);
//            auto max = m_signal->maxValueChan(i);
//            double diap = (max - min) / v * 1.2;

//            ui->wgtGraph->setDiapazone(i, (max + min) / 2 - diap / 2, (max + min) / 2 + diap / 2);
//        }
        setDiapazones(m_signalZ, ui->wgtAreaZ);
        setDiapazones(m_signalY, ui->wgtAreaY);
    }
}

void WPGraphAnalysisVisualWidget::btnFulSignalClicked(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
    {
        ui->wgtAreaZ->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
        ui->wgtAreaY->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    }
    else
    {
        ui->wgtAreaZ->setXCoordSignalMode(AreaGraph::xsm_scrolling);
        ui->wgtAreaY->setXCoordSignalMode(AreaGraph::xsm_scrolling);
    }

    setDiapazones(m_signalZ, ui->wgtAreaZ);
    setDiapazones(m_signalY, ui->wgtAreaY);
}

void WPGraphAnalysisVisualWidget::btnPlusClicked()
{
    auto hScale = ui->wgtAreaZ->hScale();
    if (hScale < 8)
        ui->wgtAreaZ->setHScale(hScale * 2);
    setDiapazones(m_signalZ, ui->wgtAreaZ);

    hScale = ui->wgtAreaY->hScale();
    if (hScale < 8)
        ui->wgtAreaY->setHScale(hScale * 2);
    setDiapazones(m_signalY, ui->wgtAreaY);
}

void WPGraphAnalysisVisualWidget::btnMinusClicked()
{
    auto hScale = ui->wgtAreaZ->hScale();
    if (hScale > 0.0625)
        ui->wgtAreaZ->setHScale(hScale / 2);
    setDiapazones(m_signalZ, ui->wgtAreaZ);

    hScale = ui->wgtAreaY->hScale();
    if (hScale > 0.0625)
        ui->wgtAreaY->setHScale(hScale / 2);
    setDiapazones(m_signalY, ui->wgtAreaY);
}

void WPGraphAnalysisVisualWidget::signalScroll(int pos)
{
    ui->wgtAreaZ->setStartPoint(ui->wgtAreaZ->area(0)->signal()->size() * pos / 100);
    ui->wgtAreaY->setStartPoint(ui->wgtAreaY->area(0)->signal()->size() * pos / 100);
    setDiapazones(m_signalZ, ui->wgtAreaZ);
    setDiapazones(m_signalY, ui->wgtAreaY);
}

void WPGraphAnalysisVisualWidget::on_signalExport()
{
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Файл для экспорта сигнала"),
                                                 DataDefines::aanalyserDocumentsPath(),
                                                 tr("Текстовые файлы") + " *.txt (*.txt)");

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);
        QStringList sl;

        sl.clear();

        for (int i = 0; i < m_signalZ->size(); ++i)
        {
            sl.clear();

            for (int j = 0; j < m_signalZ->subChansCount(); ++j)
                sl << QString::number(m_signalZ->value(j, i));
            for (int j = 0; j < m_signalY->subChansCount(); ++j)
                sl << QString::number(m_signalY->value(j, i));

            ts << sl.join("\t") + "\n";
        }

        file.close();
    }
}

void WPGraphAnalysisVisualWidget::getSignal()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelUid(), data))
        if (!m_signal)
            m_signal = new WeightPlateSignal(data);
}

void WPGraphAnalysisVisualWidget::filtration()
{
    if (m_signal)
    {
        m_fltZ.clear();
        m_fltY.clear();

        for (int i = 0; i < m_signal->size(); ++i)
        {
            m_fltZ << m_signal->value(WeightPlateSignal::scZ, i);
            m_fltY << m_signal->value(WeightPlateSignal::scY, i);
        }

        //! Фильтрация
        BaseUtils::filterLowFreq(m_fltZ, m_signal->frequency(), 2, BaseUtils::fkChebyshev, 0, m_fltZ.size() - 1);
        BaseUtils::swapVector(m_fltZ);
        BaseUtils::filterLowFreq(m_fltZ, m_signal->frequency(), 2, BaseUtils::fkChebyshev, 0, m_fltZ.size() - 1);
        BaseUtils::swapVector(m_fltZ);

        BaseUtils::filterLowFreq(m_fltY, m_signal->frequency(), 0.5, BaseUtils::fkChebyshev, 0, m_fltY.size() - 1);
        BaseUtils::swapVector(m_fltY);
        BaseUtils::filterLowFreq(m_fltY, m_signal->frequency(), 0.5, BaseUtils::fkChebyshev, 0, m_fltY.size() - 1);
        BaseUtils::swapVector(m_fltY);
    }
}

void WPGraphAnalysisVisualWidget::showGraph()
{
    if (m_signal)
    {
        m_signalZ = new DecartCoordinatesSignal(ChannelsDefines::chanAnySignalDual, m_signal->frequency());
        m_signalY = new DecartCoordinatesSignal(ChannelsDefines::chanAnySignalDual, m_signal->frequency());

        for (int i = 0; i < m_signal->size(); ++i)
        {
            double z = m_signal->value(WeightPlateSignal::scZ, i);
            double y = m_signal->value(WeightPlateSignal::scY, i);
            double fz = m_fltZ.at(i);
            double fy = m_fltY.at(i);

            m_signalZ->addValue(z, fz);
            m_signalY->addValue(y, fy);
        }

        ui->wgtAreaZ->appendSignal(m_signalZ, "");
        ui->wgtAreaY->appendSignal(m_signalY, "");

        auto minZ = m_signalZ->minValue();
        auto maxZ = m_signalZ->maxValue();
        auto minY = m_signalY->minValue();
        auto maxY = m_signalY->maxValue();

        ui->wgtAreaZ->setDiapazone(0, minZ, maxZ);
        ui->wgtAreaY->setDiapazone(0, minY, maxY);
    }
}

int WPGraphAnalysisVisualWidget::scaleMultiplier(const int scaleIdx) const
{
    int v = 1;
    for (int i = 0; i < scaleIdx; ++i)
        v *= 2;
    return v;
}

void WPGraphAnalysisVisualWidget::setDiapazones(DecartCoordinatesSignal* signal, AreaGraph* area)
{
    if (m_isCalculate)
    {
            double mid = 0;

            int n = 0;
            for (int i = area->startPoint(); i < area->startPoint() + area->areaWidth() / area->hScale(); ++i)
            {
                if (i < signal->size())
                {
                    mid += signal->value(0, i);
                    ++n;
                }
            }
            if (n > 0)
                mid /= n;

            int v = scaleMultiplier(ui->cbScale->currentIndex());
            auto min = signal->minValue();
            auto max = signal->maxValue();
            double diap = (max - min) / v * 1.2;
            area->setDiapazone(0, mid - diap / 2, mid + diap / 2);

//            if (area->areasesCount() == m_signal->subChansCount())
//            {
//                for (int i = 0; i < m_signal->subChansCount(); ++i)
//                {
//                    auto min = m_signal->minValueChan(i);
//                    auto max = m_signal->maxValueChan(i);
//                    double diap = (max - min) / v * 1.2;
//                    ui->wgtGraph->setDiapazone(i, mids.at(i) - diap / 2, mids.at(i) + diap / 2);
//                }
//            }
//            else
//            {
//                if (m_selectedChan >= 0 && m_selectedChan < m_signal->subChansCount())
//                {
//                    auto min = m_signal->minValueChan(m_selectedChan);
//                    auto max = m_signal->maxValueChan(m_selectedChan);
//                    double diap = (max - min) / v * 1.2;
//                    ui->wgtGraph->setDiapazone(0,
//                                               mids.at(m_selectedChan) - diap / 2,
//                                               mids.at(m_selectedChan) + diap / 2);
//                }
//            }
    }
}
