#include "stepdeviationvisualize.h"
#include "ui_stepdeviationvisualize.h"

#include "basedefines.h"
#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "stepdeviationcalculator.h"
#include "reportelements.h"
#include "decartcoordinatessignal.h"
#include "dynamicdiagrampainter.h"
#include "stepdeviationfactors.h"

#include <QPainter>
#include <QDebug>

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

        connect(ui->wgtGrowth, &DynamicDiagram::selectItem, this, &StepDeviationVisualize::on_selectItemGrowth);
        connect(ui->wgtLength, &DynamicDiagram::selectItem, this, &StepDeviationVisualize::on_selectItemLength);
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
        //! График
        auto rect = QRect(paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 14 * 2.1),
                          static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.1));
        double ratio = ReportElements::ratio(paper, visual->m_wgtGraph, 5);
        printGraph(painter, rect, visual, ratio);

        //! Диаграммы
        rect = QRect(paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() * 0.26),
                          static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.1));
        ratio = ReportElements::ratio(paper, visual->m_wgtGrowth, 2);
        QVector<double> bars;
        QStringList labels;
        for (int i = 0; i < visual->m_calculator->growthDynCount(); ++i)
        {
            bars << visual->m_calculator->growthDynValue(i);
            labels << "";
        }
        ReportElements::drawDynamicDiag(painter, rect, ratio, bars, labels, tr("Динамика прироста"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);

        rect = QRect(paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 14 * 5.2),
                     static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.1));
        bars.clear();
        labels.clear();
        for (int i = 0; i < visual->m_calculator->lengthDynCount(); ++i)
        {
            bars << visual->m_calculator->lengthDynValue(i);
            labels << "";
        }
        ReportElements::drawDynamicDiag(painter, rect, ratio, bars, labels, tr("Динамика длительности отклонений"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);

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
        //! График
        auto rect = QRect(paper.x() + paper.width()/20, static_cast<int>(paper.y() + paper.height() * 0.15),
                          static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.26));
        double ratio = ReportElements::ratio(paper, visual->m_wgtGraph, 5);
        printGraph(painter, rect, visual, ratio);

        //! Диаграммы
        rect = QRect(paper.x() + paper.width()/20, static_cast<int>(paper.y() + paper.height() * 0.45),
                          static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.2));
        ratio = ReportElements::ratio(paper, visual->m_wgtGrowth, 2);
        QVector<double> bars;
        QStringList labels;
        for (int i = 0; i < visual->m_calculator->growthDynCount(); ++i)
        {
            bars << visual->m_calculator->growthDynValue(i);
            labels << "";
        }
        ReportElements::drawDynamicDiag(painter, rect, ratio, bars, labels, tr("Динамика прироста"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);

        rect = QRect(paper.x() + paper.width()/20, static_cast<int>(paper.y() + paper.height() * 0.68),
                     static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.2));
        bars.clear();
        labels.clear();
        for (int i = 0; i < visual->m_calculator->lengthDynCount(); ++i)
        {
            bars << visual->m_calculator->lengthDynValue(i);
            labels << "";
        }
        ReportElements::drawDynamicDiag(painter, rect, ratio, bars, labels, tr("Динамика длительности отклонений"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);

        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 60 * 56, visual->m_directionText);

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

void StepDeviationVisualize::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, StepDeviationCalculator *calculator)
{
    Q_UNUSED(testUid);

    const QStringList DisplayFactors = QStringList() << StepDeviationFactorsDefines::TimeUid
                                                     << StepDeviationFactorsDefines::StepCountUid
                                                     << StepDeviationFactorsDefines::ErrorCountUid
                                                     << StepDeviationFactorsDefines::GrowthAvrgUid;

    painter->save();

    int size = 0;
    int z = 6;
    if (rect.width() > rect.height())
        size = (rect.height() - z) / 2;
    else
        size = (rect.width() - z) / 2;

    //! Значения показателей
    int n = 0;
    painter->setFont(QFont("Sans", 7, QFont::Bold, false));
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    for (int i = 0; i < calculator->primaryFactorsCount(); ++i)
    {
        auto factor = calculator->primaryFactor(i);
        if (DisplayFactors.contains(factor->uid()))
        {
            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factor->uid());
            QString fn = fi.shortName();
            if (fi.measure() != "")
                fn = fn + ", " + fi.measure();
            painter->drawText(rect.x() + 4, rect.y() + 7 + n * 10, fn);
            painter->drawText(rect.x() + 75, rect.y() + 7 + n * 10, QString::number(factor->value(), 'f', fi.format()));

            ++n;
        }
    }

    painter->restore();
}

void StepDeviationVisualize::on_selectItemGrowth(const int idx)
{
    if (idx >= 0 && idx < ui->wgtGrowth->itemCount())
    {
        double v = m_calculator->growthDynValue(idx);
        ui->wgtGrowth->setTitle(tr("Динамика прироста, мм") + " (" + QString::number(v, 'f', 2) + " мм)");
    }
}

void StepDeviationVisualize::on_selectItemLength(const int idx)
{
    if (idx >= 0 && idx < ui->wgtLength->itemCount())
    {
        double v = m_calculator->lengthDynValue(idx);
        ui->wgtLength->setTitle(tr("Динамика длительности отклонений, сек") + " (" + QString::number(v) + " сек)");
    }
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
    gp.appendSignal(signal, "Прирост");

    double max = signal->maxValue();
    double min = signal->minValue();
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

    ui->wgtGrowth->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtGrowth->setVolume(DynamicDiagramDefines::Volume3D);
    ui->wgtGrowth->setTitle(tr("Динамика прироста"));
    ui->wgtGrowth->setAxisSpaceLeft(30);
    ui->wgtGrowth->setAxisSpaceBottom(10);
    m_wgtGrowth = ui->wgtGrowth;

    for (int i = 0; i < m_calculator->growthDynCount(); ++i)
    {
        auto item = new DiagItem(m_calculator->growthDynValue(i), tr(""));
        ui->wgtGrowth->appendItem(item);
    }

    ui->wgtLength->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtLength->setVolume(DynamicDiagramDefines::Volume3D);
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
    m_directionText = tr("Направление отклонений") + " - " + BaseDefines::DirectionValueFBName.value(m_calculator->direction());
    ui->lblDirection->setText(m_directionText);
}
