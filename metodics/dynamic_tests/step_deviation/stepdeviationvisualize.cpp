#include "stepdeviationvisualize.h"
#include "ui_stepdeviationvisualize.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "stepdeviationcalculator.h"
#include "reportelements.h"
#include "decartcoordinatessignal.h"

#include <QPainter>

namespace
{
AreaGraph *wgtGraph {nullptr};
DynamicDiagram *wgtGrowth {nullptr};
DynamicDiagram *wgtLength {nullptr};
QStandardItemModel *mdlFactors {nullptr};
QString directionText {""};
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
        ReportElements::drawWidget(painter, wgtGraph,
                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 14 * 2.3));
        ReportElements::drawWidget(painter, wgtGrowth,
                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
                                   paper.x() + paper.width()/10, paper.y() + paper.height() / 14 * 4);
        ReportElements::drawWidget(painter, wgtLength,
                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 14 * 5.3));

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 2,
                        paper.width() / 10 * 8,
                        paper.height() / 5 * 2);
        ReportElements::drawTable(painter, mdlFactors, rectTable,
                                  QList<int>() << 3 << 1, ReportElements::Table::tvsStretched,
                                  12, -1, QFont::Bold);

        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 60 * 55, directionText);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Диаграмма. Копируется из виджета
        ReportElements::drawWidget(painter, wgtGraph,
                                   static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.9),
                                   paper.x() + paper.width()/20, paper.y() + paper.height()/6);
        ReportElements::drawWidget(painter, wgtGrowth,
                                   static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.9),
                                   paper.x() + paper.width()/20, static_cast<int>(paper.y() + paper.height() / 6 * 2.6));
        ReportElements::drawWidget(painter, wgtLength,
                                   static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.9),
                                   paper.x() + paper.width()/20, static_cast<int>(paper.y() + paper.height() / 6 * 4));

        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 60 * 55, directionText);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 2
        printer->newPage();

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 7),
                        paper.width() / 10 * 9,
                        paper.height() / 10 * 8);
        ReportElements::drawTable(painter, mdlFactors, rectTable,
                                  QList<int>() << 3 << 1, ReportElements::Table::tvsStretched,
                                  12, -1, QFont::Bold);
    }

    //! Нижний колонтитул
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
        rec.y = m_calculator->signal(i);
//        rec.y = m_calculator->signalFiltred(i);
        signal->addValue(rec);
    }

    double max = signal->maxValue();
    double min = signal->minValue();
    ui->wgtGraph->appendSignal(signal, tr("Прирост"));
    ui->wgtGraph->setDiapazone(0, min, max);

    wgtGraph = ui->wgtGraph;
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

    mdlFactors = static_cast<QStandardItemModel*>(ui->tvFactors->model());
}

void StepDeviationVisualize::showDiags()
{

    ui->wgtGrowth->setKind(DynamicDiagram::KindBar);
    ui->wgtGrowth->setVolume(DynamicDiagram::Volume3D);
    ui->wgtGrowth->setTitle(tr("Динамика прироста"));
    ui->wgtGrowth->setAxisSpaceLeft(30);
    ui->wgtGrowth->setAxisSpaceBottom(10);
    wgtGrowth = ui->wgtGrowth;

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
    wgtLength = ui->wgtLength;

    for (int i = 0; i < m_calculator->lengthDynCount(); ++i)
    {
        auto item = new DiagItem(m_calculator->lengthDynValue(i), tr(""));
        ui->wgtLength->appendItem(item);
    }
}

void StepDeviationVisualize::showDirection()
{
    directionText = tr("Направление отклонений") + " - " + BaseUtils::DirectionValueName.value(m_calculator->direction());
    ui->lblDirection->setText(directionText);
}
