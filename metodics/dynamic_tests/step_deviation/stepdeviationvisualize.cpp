#include "stepdeviationvisualize.h"
#include "ui_stepdeviationvisualize.h"

#include "channelsdefines.h"
#include "stepdeviationcalculator.h"
#include "reportelements.h"
#include "decartcoordinatessignal.h"

#include <QPainter>

StepDeviationVisualize::StepDeviationVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StepDeviationVisualize)
{
    ui->setupUi(this);
}

StepDeviationVisualize::~StepDeviationVisualize()
{
    delete ui;
}

void StepDeviationVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new StepDeviationCalculator(testUid, this);
        m_calculator->calculate();

        showGraph();
    }
}

void StepDeviationVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
//        //! Диаграмма Cross. Копируется из виджета
//        ReportElements::drawWidget(painter, wgtDiag,
//                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
//                                   paper.x() + paper.width()/10, paper.y() + paper.height()/7);

//        //! Таблица показателей. Берется модель таблицы из визуализатора
//        QRect rectTable(paper.x() + paper.width() / 10,
//                        paper.y() + paper.height() / 3 * 2,
//                        paper.width() / 10 * 8,
//                        paper.height() / 4);
//        ReportElements::drawTable(painter, mdlFactors, rectTable,
//                                  QList<int>() << 3 << 1, ReportElements::Table::tvsStretched,
//                                  12, -1, QFont::Bold);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
//        //! Диаграмма Cross. Копируется из виджета
//        ReportElements::drawWidget(painter, wgtDiag,
//                                   static_cast<int>(paper.width() * 0.6), static_cast<int>(paper.height() * 0.6),
//                                   paper.x() + paper.width()/20, paper.y() + paper.height()/4);

//        //! Таблица показателей. Берется модель таблицы из визуализатора
//        QRect rectTable(paper.x() + paper.width() / 7 * 4,
//                        static_cast<int>(paper.y() + paper.height() / 3.5),
//                        paper.width() / 7 * 3,
//                        paper.height() / 2);
//        ReportElements::drawTable(painter, mdlFactors, rectTable,
//                                  QList<int>() << 3 << 1, ReportElements::Table::tvsStretched,
//                                  12, -1, QFont::Bold);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void StepDeviationVisualize::showGraph()
{
    auto signal = new DecartCoordinatesSignal(ChannelsDefines::chanAnySignalDual, m_calculator->signalFrequency());
    for (int i = 0; i < m_calculator->signalSize(); ++i)
    {
        SignalsDefines::StabRec rec;
        rec.x = m_calculator->signal(i);
        rec.y = m_calculator->signalFiltred(i);
        signal->addValue(rec);
    }

    double max = signal->maxValue();
    double min = signal->minValue();
    ui->wgtGraph->appendSignal(signal, tr("Прирост"));
    ui->wgtGraph->setDiapazone(0, min, max);

}
