#include "trianglevisualize.h"
#include "ui_trianglevisualize.h"

#include <QPainter>
#include <QDebug>

#include "channelsdefines.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "settingsprovider.h"
#include "trianglecalculator.h"
#include "reportelements.h"
#include "stabilogram.h"
#include "areaskgdefines.h"

TriangleVisualize::TriangleVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriangleVisualize)
{
    ui->setupUi(this);

    auto val = SettingsProvider::valueFromRegAppCopy("TriangleResultWidget", "CurrentPage").toInt();
    ui->twPages->setCurrentIndex(val);

    restoreSplitterPositionDiag();
}

TriangleVisualize::~TriangleVisualize()
{
    delete ui;
}

void TriangleVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new TriangleCalculator(testUid, this);
        m_calculator->calculate();

        getSignal(testUid);

        showSKG(ui->wgtSKGTraining, BaseUtils::Section(0, m_calculator->trainingLength()));
        showSKG(ui->wgtSKGAnalysis, BaseUtils::Section(m_calculator->trainingLength(), m_calculator->signalLength()));
        qDebug() << m_calculator->triangleTraining().topCorner.x()
                 << m_calculator->triangleTraining().topCorner.y()
                 << m_calculator->triangleTraining().leftDownCorner.x()
                 << m_calculator->triangleTraining().leftDownCorner.y()
                 << m_calculator->triangleTraining().rightDownCorner.x()
                 << m_calculator->triangleTraining().rightDownCorner.y();
        addTriangleDiag(ui->wgtSKGTraining, m_calculator->triangleTraining(), Qt::darkCyan);
        addTriangleDiag(ui->wgtSKGAnalysis, m_calculator->triangleAnalysis(), Qt::darkCyan);

        m_curTriangleAnalysis = m_calculator->firstAnalysisTriangle();
        setBtnPNTrainingEnabled();
        setBtnPNAnalysisEnabled();
    }
}

void TriangleVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
//        //! Диаграмма. Копируется из виджета
//        ReportElements::drawWidget(painter, wgtDiag,
//                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
//                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height()/4));
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
//        //! Диаграмма. Копируется из виджета
//        ReportElements::drawWidget(painter, wgtDiag,
//                                   static_cast<int>(paper.width() * 0.75), static_cast<int>(paper.height() * 0.75),
//                                   static_cast<int>(paper.x() + paper.width()/5.5), paper.y() + paper.height()/6);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void TriangleVisualize::curPageChanged(int pageIdx)
{
    SettingsProvider::setValueToRegAppCopy("TriangleResultWidget", "CurrentPage", pageIdx);
}

void TriangleVisualize::splitterDiagMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPositionDiag();
    //    setSKGSize();
}

void TriangleVisualize::btnAllTrainingClicked(bool pressed)
{
    if (pressed)
        ui->wgtSKGTraining->setSection(0, m_calculator->trainingLength());
    else
        ui->wgtSKGTraining->setSection(m_calculator->triangleSection(m_curTriangleTraining).begin,
                                       m_calculator->triangleSection(m_curTriangleTraining).end);
    setBtnPNTrainingEnabled();
}

void TriangleVisualize::btnAllAnalysisClicked(bool pressed)
{
    if (pressed)
        ui->wgtSKGAnalysis->setSection(m_calculator->trainingLength(), m_calculator->signalLength());
    else
        ui->wgtSKGAnalysis->setSection(m_calculator->triangleSection(m_curTriangleAnalysis).begin,
                                       m_calculator->triangleSection(m_curTriangleAnalysis).end);
    setBtnPNAnalysisEnabled();
}

void TriangleVisualize::btnPrevTrainingClicked()
{
    if (m_curTriangleTraining > 0)
    {
        --m_curTriangleTraining;
        ui->wgtSKGTraining->setSection(m_calculator->triangleSection(m_curTriangleTraining).begin,
                                       m_calculator->triangleSection(m_curTriangleTraining).end);
    }
    setBtnPNTrainingEnabled();
}

void TriangleVisualize::btnNextTrainingClicked()
{
    if (m_curTriangleTraining < m_calculator->firstAnalysisTriangle() - 1)
    {
        ++m_curTriangleTraining;
        ui->wgtSKGTraining->setSection(m_calculator->triangleSection(m_curTriangleTraining).begin,
                                       m_calculator->triangleSection(m_curTriangleTraining).end);
    }
    setBtnPNTrainingEnabled();
}

void TriangleVisualize::setBtnPNTrainingEnabled()
{
    ui->tbPrevTraining->setEnabled(!ui->tbShowAllTraining->isChecked() &&
                                   m_curTriangleTraining > 0);
    ui->tbNextTraining->setEnabled(!ui->tbShowAllTraining->isChecked() &&
                                   m_curTriangleTraining < m_calculator->firstAnalysisTriangle() - 1);
    ui->lblTrnglNumTraining->setText(QString::number(m_curTriangleTraining + 1));
}

void TriangleVisualize::btnPrevAnalysisClicked()
{
    if (m_curTriangleAnalysis > m_calculator->firstAnalysisTriangle())
    {
        --m_curTriangleAnalysis;
        ui->wgtSKGAnalysis->setSection(m_calculator->triangleSection(m_curTriangleAnalysis).begin,
                                       m_calculator->triangleSection(m_curTriangleAnalysis).end);
    }
    setBtnPNAnalysisEnabled();
}

void TriangleVisualize::btnNextAnalysisClicked()
{
    if (m_curTriangleAnalysis < m_calculator->trianglesCount() - 1)
    {
        ++m_curTriangleAnalysis;
        ui->wgtSKGAnalysis->setSection(m_calculator->triangleSection(m_curTriangleAnalysis).begin,
                                       m_calculator->triangleSection(m_curTriangleAnalysis).end);
    }
    setBtnPNAnalysisEnabled();
}

void TriangleVisualize::setBtnPNAnalysisEnabled()
{
    ui->tbPrevAnalysis->setEnabled(!ui->tbShowAllAnalysis->isChecked() &&
                                   m_curTriangleAnalysis > m_calculator->firstAnalysisTriangle());
    ui->tbNextAnalysis->setEnabled(!ui->tbShowAllAnalysis->isChecked() &&
                                   m_curTriangleAnalysis < m_calculator->trianglesCount() - 1);
    ui->lblTrnglNumAnalysis->setText(QString::number(m_curTriangleAnalysis - m_calculator->firstAnalysisTriangle() + 1));
}

void TriangleVisualize::getSignal(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab) &&
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanTriangleResult))
                {
                    QByteArray baStab;
                    if (DataProvider::getChannel(pi.uid, ChannelsDefines::chanStab, baStab))
                        m_stab = new Stabilogram(baStab);
                }
        }
}

void TriangleVisualize::showSKG(AreaSKG *area, BaseUtils::Section section)
{
    area->setSignal(m_stab, section.begin, section.end);
    addTriangleDiag(area, m_calculator->triangleOriginal(), Qt::darkYellow);
}

int TriangleVisualize::addTriangleDiag(AreaSKG *area, TriangleDefines::Triangle triangle, QColor color)
{
    AreaSKGDefines::BrokenLine blTrngl;
    blTrngl.polygon << QPointF(triangle.topCorner.x(), triangle.topCorner.y())
                    << QPointF(triangle.leftDownCorner.x(), triangle.leftDownCorner.y())
                    << QPointF(triangle.rightDownCorner.x(), triangle.rightDownCorner.y());
    blTrngl.color = color;
    blTrngl.width = 3;
    area->addBrokenLine(blTrngl);
}

void TriangleVisualize::saveSplitterPositionDiag()
{
    SettingsProvider::setValueToRegAppCopy("TriangleResultWidget", "SplitterDiagramPosition", ui->splDiag->saveState());
    SettingsProvider::setValueToRegAppCopy("TriangleResultWidget", "SplitterDiagramAreasesPosition", ui->splDiagAreases->saveState());
}

void TriangleVisualize::restoreSplitterPositionDiag()
{
    auto valDiag = SettingsProvider::valueFromRegAppCopy("TriangleResultWidget", "SplitterDiagramPosition").toByteArray();
    ui->splDiag->restoreState(valDiag);
    auto valDiagAreases = SettingsProvider::valueFromRegAppCopy("TriangleResultWidget", "SplitterDiagramAreasesPosition").toByteArray();
    ui->splDiagAreases->restoreState(valDiagAreases);
}

