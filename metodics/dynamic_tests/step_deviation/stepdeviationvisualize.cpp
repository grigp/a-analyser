#include "stepdeviationvisualize.h"
#include "ui_stepdeviationvisualize.h"

#include "basedefines.h"
#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "stepdeviationcalculator.h"
#include "reportelements.h"
#include "decartcoordinatessignal.h"

#include <QPainter>
#include <QDebug>

namespace
{
//AreaGraph *m_wgtGraph {nullptr};
//DynamicDiagram *m_wgtGrowth {nullptr};
//DynamicDiagram *m_wgtLength {nullptr};
//QStandardItemModel *m_mdlFactors {nullptr};
//QString m_directionText {""};
}

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
        showTable();
        showDiags();
        showDirection();
    }
}

void StepDeviationVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    //! Получаем указатель на элземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    StepDeviationVisualize* visual = static_cast<StepDeviationVisualize*>(vis);

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Диаграмма. Копируется из виджета
        auto rect = QRect(paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 14 * 2.3),
                          static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.1));
        double ratio = ReportElements::ratio(paper, visual->m_wgtGraph, 5);
        printGraph(painter, rect, visual, ratio);

//        ReportElements::drawWidget(painter, visual->m_wgtGraph,
//                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
//                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 14 * 2.3));
        ReportElements::drawWidget(painter, visual->m_wgtGrowth,
                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
                                   paper.x() + paper.width()/10, paper.y() + paper.height() / 14 * 4);
        ReportElements::drawWidget(painter, visual->m_wgtLength,
                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 14 * 5.3));

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 2,
                        paper.width() / 10 * 8,
                        paper.height() / 5 * 2);
        ReportElements::drawTable(painter, visual->m_mdlFactors, rectTable,
                                  QList<int>() << 3 << 1, false, ReportElements::Table::tvsStretched,
                                  12, -1, QFont::Bold);

        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 60 * 55, visual->m_directionText);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Диаграмма. Копируется из виджета
        ReportElements::drawWidget(painter, visual->m_wgtGraph,
                                   static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.9),
                                   paper.x() + paper.width()/20, paper.y() + paper.height()/6);
        ReportElements::drawWidget(painter, visual->m_wgtGrowth,
                                   static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.9),
                                   paper.x() + paper.width()/20, static_cast<int>(paper.y() + paper.height() / 6 * 2.6));
        ReportElements::drawWidget(painter, visual->m_wgtLength,
                                   static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.9),
                                   paper.x() + paper.width()/20, static_cast<int>(paper.y() + paper.height() / 6 * 4));

        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 60 * 55, visual->m_directionText);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 2
        printer->newPage();

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 7),
                        paper.width() / 10 * 9,
                        paper.height() / 10 * 8);
        ReportElements::drawTable(painter, visual->m_mdlFactors, rectTable,
                                  QList<int>() << 3 << 1, false, ReportElements::Table::tvsStretched,
                                  12, -1, QFont::Bold);
    }

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void StepDeviationVisualize::printGraph(QPainter *painter, const QRect &rect, StepDeviationVisualize *visual, double ratio)
{
    auto signal = new DecartCoordinatesSignal(ChannelsDefines::chanAnySignalDual, visual->m_calculator->signalFrequency());
    for (int i = 0; i < visual->m_calculator->signalSize(); ++i)
    {
        SignalsDefines::StabRec rec;
        rec.x = visual->m_calculator->signal(i);
        rec.y = visual->m_calculator->signal(i);
        signal->addValue(rec);
    }

    //! Создаем рисователь
    GraphPainter gp(painter, rect);
    //! Передаем его в рисователь
    gp.appendSignal(signal, "");

    double max = signal->maxValue();
    double min = signal->minValue();
    gp.appendSignal(signal, tr("Прирост"));
    gp.setDiapazone(0, min, max);
    gp.doPaint(ratio);
}

void StepDeviationVisualize::showGraph()
{
    auto signal = new DecartCoordinatesSignal(ChannelsDefines::chanAnySignalDual, m_calculator->signalFrequency());
    for (int i = 0; i < m_calculator->signalSize(); ++i)
    {
        SignalsDefines::StabRec rec;
        rec.x = m_calculator->signal(i);
        rec.y = m_calculator->signal(i);
//        rec.y = m_calculator->signalFiltred(i);
        signal->addValue(rec);
    }

    double max = signal->maxValue();
    double min = signal->minValue();
    ui->wgtGraph->appendSignal(signal, tr("Прирост"));
    ui->wgtGraph->setDiapazone(0, min, max);

    m_wgtGraph = ui->wgtGraph;
}

void StepDeviationVisualize::showTable()
{
    auto model = new QStandardItemModel();
    for (int i = 0; i < m_calculator->primaryFactorsCount(); ++i)
    {
        auto factor = m_calculator->primaryFactor(i);
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factor->uid());
        QString fn = fi.name();
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto *itemName = new QStandardItem(fn);
        itemName->setEditable(false);
        auto *itemValue = new QStandardItem(QString::number(factor->value(), 'f', fi.format()));
        itemValue->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemValue);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель")
                                                   << tr("Значение"));
    ui->tvFactors->setModel(model);
    ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvFactors->header()->resizeSection(0, 450);

    m_mdlFactors = static_cast<QStandardItemModel*>(ui->tvFactors->model());
}

void StepDeviationVisualize::showDiags()
{

    ui->wgtGrowth->setKind(DynamicDiagram::KindBar);
    ui->wgtGrowth->setVolume(DynamicDiagram::Volume3D);
    ui->wgtGrowth->setTitle(tr("Динамика прироста"));
    ui->wgtGrowth->setAxisSpaceLeft(30);
    ui->wgtGrowth->setAxisSpaceBottom(10);
    m_wgtGrowth = ui->wgtGrowth;

    for (int i = 0; i < m_calculator->growthDynCount(); ++i)
    {
        auto item = new DiagItem(m_calculator->growthDynValue(i), tr(""));
        ui->wgtGrowth->appendItem(item);
    }

    ui->wgtLength->setKind(DynamicDiagram::KindBar);
    ui->wgtLength->setVolume(DynamicDiagram::Volume3D);
    ui->wgtLength->setTitle(tr("Динамика длительности отклонений"));
    ui->wgtLength->setAxisSpaceLeft(30);
    ui->wgtLength->setAxisSpaceBottom(10);
    m_wgtLength = ui->wgtLength;

    for (int i = 0; i < m_calculator->lengthDynCount(); ++i)
    {
        auto item = new DiagItem(m_calculator->lengthDynValue(i), tr(""));
        ui->wgtLength->appendItem(item);
    }
}

void StepDeviationVisualize::showDirection()
{
    m_directionText = tr("Направление отклонений") + " - " + BaseDefines::DirectionValueName.value(m_calculator->direction());
    ui->lblDirection->setText(m_directionText);
}
