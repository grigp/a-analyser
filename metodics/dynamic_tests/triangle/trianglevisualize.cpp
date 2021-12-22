#include "trianglevisualize.h"
#include "ui_trianglevisualize.h"

#include <QPainter>

#include "channelsdefines.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "settingsprovider.h"
#include "trianglecalculator.h"
#include "reportelements.h"
#include "stabilogram.h"

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

