#include "boxerdodgingvisualize.h"
#include "ui_boxerdodgingvisualize.h"

#include "aanalyserapplication.h"
#include "boxerdodgingcalculator.h"
#include "boxerdodgingmultifactor.h"
#include "dynamicdiagram.h"
#include "reportelements.h"

#include <QPainter>
#include <QDebug>

BoxerDodgingVisualize::BoxerDodgingVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoxerDodgingVisualize)
{
    ui->setupUi(this);
}

BoxerDodgingVisualize::~BoxerDodgingVisualize()
{
    delete ui;
}

void BoxerDodgingVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new BoxerDodgingCalculator(testUid, this);
        m_calculator->calculate();

        showDiagram(ui->wgtDiagLatent,
                    tr("Латентный период, сек"),
                    BoxerDodgingFactorsDefines::LeftLatUid,
                    BoxerDodgingFactorsDefines::RightLatUid,
                    BoxerDodgingFactorsDefines::AheadLatUid,
                    BoxerDodgingFactorsDefines::BackLatUid);
        showDiagram(ui->wgtDiagTime,
                    tr("Время реакции. сек"),
                    BoxerDodgingFactorsDefines::LeftTimeUid,
                    BoxerDodgingFactorsDefines::RightTimeUid,
                    BoxerDodgingFactorsDefines::AheadTimeUid,
                    BoxerDodgingFactorsDefines::BackTimeUid);
        showDiagram(ui->wgtDiagAmpl,
                    tr("Амплитуда, мм"),
                    BoxerDodgingFactorsDefines::LeftAmplUid,
                    BoxerDodgingFactorsDefines::RightAmplUid,
                    BoxerDodgingFactorsDefines::AheadAmplUid,
                    BoxerDodgingFactorsDefines::BackAmplUid);
        showDiagram(ui->wgtDiagErrors,
                    tr("Ошибки"),
                    BoxerDodgingFactorsDefines::LeftErrorsUid,
                    BoxerDodgingFactorsDefines::RightErrorsUid,
                    BoxerDodgingFactorsDefines::AheadErrorsUid,
                    BoxerDodgingFactorsDefines::BackErrorsUid);

        showTable();

        m_wgtDiagLatent = ui->wgtDiagLatent;
        m_wgtDiagTime = ui->wgtDiagTime;
        m_wgtDiagAmpl = ui->wgtDiagAmpl;
        m_wgtDiagErrors = ui->wgtDiagErrors;
    }
}


void BoxerDodgingVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    //! Получаем указатель на элземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    BoxerDodgingVisualize* visual = static_cast<BoxerDodgingVisualize*>(vis);

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        auto rectLat = QRect(paper.x() + paper.width()/10, paper.y() + paper.height()/7,
                             static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.25));
        auto ratioLat = ReportElements::ratio(paper, visual->m_wgtDiagLatent, 2);
        auto barsLat = QVector<double>() << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::LeftLatUid)
                                         << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::RightLatUid)
                                         << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::AheadLatUid)
                                         << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::BackLatUid);
        auto labelsLat = QStringList() << tr("Влево") << tr("Вправо") << tr("Вперед") << tr("Назад");
        ReportElements::drawDynamicDiag(painter, rectLat, ratioLat, barsLat, labelsLat, tr("Латентный период, сек"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);

        auto rectTime = QRect(paper.x() + paper.width()/11*6, paper.y() + paper.height()/7,
                              static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.25));
        auto ratioTime = ReportElements::ratio(paper, visual->m_wgtDiagTime, 2);
        auto barsTime = QVector<double>() << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::LeftTimeUid)
                                          << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::RightTimeUid)
                                          << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::AheadTimeUid)
                                          << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::BackTimeUid);
        auto labelsTime = QStringList() << tr("Влево") << tr("Вправо") << tr("Вперед") << tr("Назад");
        ReportElements::drawDynamicDiag(painter, rectTime, ratioTime, barsTime, labelsTime, tr("Время реакции, сек"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);

        auto rectAmpl = QRect(paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() * 0.42),
                              static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.25));
        auto ratioAmpl = ReportElements::ratio(paper, visual->m_wgtDiagLatent, 2);
        auto barsAmpl = QVector<double>() << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::LeftAmplUid)
                                          << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::RightAmplUid)
                                          << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::AheadAmplUid)
                                          << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::BackAmplUid);
        auto labelsAmpl = QStringList() << tr("Влево") << tr("Вправо") << tr("Вперед") << tr("Назад");
        ReportElements::drawDynamicDiag(painter, rectAmpl, ratioAmpl, barsAmpl, labelsAmpl, tr("Амплитуда, мм"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);

        auto rectErr = QRect(paper.x() + paper.width()/11*6, static_cast<int>(paper.y() + paper.height() * 0.42),
                             static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.25));
        auto ratioErr = ReportElements::ratio(paper, visual->m_wgtDiagLatent, 2);
        auto barsErr = QVector<double>() << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::LeftErrorsUid)
                                         << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::RightErrorsUid)
                                         << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::AheadErrorsUid)
                                         << visual->m_calculator->factorValue(BoxerDodgingFactorsDefines::BackErrorsUid);
        auto labelsErr = QStringList() << tr("Влево") << tr("Вправо") << tr("Вперед") << tr("Назад");
        ReportElements::drawDynamicDiag(painter, rectErr, ratioErr, barsErr, labelsErr, tr("Ошибки"),
                                        DynamicDiagramDefines::KindBar, DynamicDiagramDefines::Volume3D);

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 18 * 13,
                        paper.width() / 10 * 8,
                        paper.height() / 5);
        ReportElements::drawTable(painter, visual->m_mdlFactors, rectTable, QList<int>() << 3 << 3 << 3 << 3 << 2 << 1,
                                  false, ReportElements::Table::tvsStretched, 11, -1, QFont::Bold);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        ReportElements::drawWidget(painter, visual->m_wgtDiagLatent,
                                   static_cast<int>(paper.width() * 0.23), static_cast<int>(paper.height() * 0.5),
                                   paper.x() + paper.width()/20, paper.y() + paper.height()/5);
        ReportElements::drawWidget(painter, visual->m_wgtDiagTime,
                                   static_cast<int>(paper.width() * 0.23), static_cast<int>(paper.height() * 0.5),
                                   paper.x() + paper.width()/20*5, paper.y() + paper.height()/5);
        ReportElements::drawWidget(painter, visual->m_wgtDiagAmpl,
                                   static_cast<int>(paper.width() * 0.23), static_cast<int>(paper.height() * 0.5),
                                   paper.x() + paper.width()/20 * 10, paper.y() + paper.height()/5);
        ReportElements::drawWidget(painter, visual->m_wgtDiagErrors,
                                   static_cast<int>(paper.width() * 0.23), static_cast<int>(paper.height() * 0.5),
                                   paper.x() + paper.width()/20*15, paper.y() + paper.height()/5);

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 18 * 11,
                        paper.width() / 10 * 8,
                        paper.height() / 5);
        ReportElements::drawTable(painter, visual->m_mdlFactors, rectTable, QList<int>() << 3 << 3 << 3 << 3 << 2 << 1,
                                  false, ReportElements::Table::tvsStretched, 11, -1, QFont::Bold);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}


void BoxerDodgingVisualize::showDiagram(DynamicDiagram* diag, const QString title,
                                        const QString leftFctUid, const QString rightFctUid, const QString aheadFctUid, const QString backFctUid)
{
    diag->setKind(DynamicDiagramDefines::KindBar);
    diag->setVolume(DynamicDiagramDefines::Volume3D);
    diag->setTitle(title);

    diag->setAxisSpaceLeft(30);

    auto itemL = new DiagItem(m_calculator->factorValue(leftFctUid), tr("Влево"));
    diag->appendItem(itemL);
    auto itemR = new DiagItem(m_calculator->factorValue(rightFctUid), tr("Вправо"));
    diag->appendItem(itemR);
    auto itemA = new DiagItem(m_calculator->factorValue(aheadFctUid), tr("Вперед"));
    diag->appendItem(itemA);
    auto itemB = new DiagItem(m_calculator->factorValue(backFctUid), tr("Назад"));
    diag->appendItem(itemB);
}

void BoxerDodgingVisualize::showTable()
{
    auto *model = new QStandardItemModel(ui->tvTable);

    model->appendRow(
                getLineItems(tr("Влево"),
                             BoxerDodgingFactorsDefines::LeftLatUid,
                             BoxerDodgingFactorsDefines::LeftTimeUid,
                             BoxerDodgingFactorsDefines::LeftAmplUid,
                             BoxerDodgingFactorsDefines::LeftErrorsUid,
                             BoxerDodgingDefines::bdsLeftDodging));
    model->appendRow(
                getLineItems(tr("Вправо"),
                             BoxerDodgingFactorsDefines::RightLatUid,
                             BoxerDodgingFactorsDefines::RightTimeUid,
                             BoxerDodgingFactorsDefines::RightAmplUid,
                             BoxerDodgingFactorsDefines::RightErrorsUid,
                             BoxerDodgingDefines::bdsRightDodging));
    model->appendRow(
                getLineItems(tr("Вперед"),
                             BoxerDodgingFactorsDefines::AheadLatUid,
                             BoxerDodgingFactorsDefines::AheadTimeUid,
                             BoxerDodgingFactorsDefines::AheadAmplUid,
                             BoxerDodgingFactorsDefines::AheadErrorsUid,
                             BoxerDodgingDefines::bdsAheadBend));
    model->appendRow(
                getLineItems(tr("Назад"),
                             BoxerDodgingFactorsDefines::BackLatUid,
                             BoxerDodgingFactorsDefines::BackTimeUid,
                             BoxerDodgingFactorsDefines::BackAmplUid,
                             BoxerDodgingFactorsDefines::BackErrorsUid,
                             BoxerDodgingDefines::bdsBackBend));
    model->appendRow(
                getLineItems(tr("Среднее / Сумма"),
                             BoxerDodgingFactorsDefines::AverageLatUid,
                             BoxerDodgingFactorsDefines::AverageTimeUid,
                             BoxerDodgingFactorsDefines::AverageAmplUid,
                             BoxerDodgingFactorsDefines::AllErrorsUid,
                             BoxerDodgingDefines::bdsBase));


    model->setHorizontalHeaderLabels(QStringList() << tr("Этап")
                                                   << tr("Лат.период")
                                                   << tr("Время реакции")
                                                   << tr("Амплитуда")
                                                   << tr("Попытки")
                                                   << tr("Ошибки"));
    ui->tvTable->setModel(model);
    ui->tvTable->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvTable->header()->resizeSection(0, 250);

    m_mdlFactors = static_cast<QStandardItemModel*>(ui->tvTable->model());
}


QList<QStandardItem *> BoxerDodgingVisualize::getLineItems(const QString title,
                                                           const QString latFctUid,
                                                           const QString timeFctUid,
                                                           const QString amplFctUid,
                                                           const QString errFctUid,
                                                           const BoxerDodgingDefines::Stages stage)
{
    auto *itemName = new QStandardItem(title);
    itemName->setEditable(false);
    auto *itemLat = new QStandardItem(m_calculator->factorValueFormatted(latFctUid));
    itemLat->setEditable(false);
    auto *itemTime = new QStandardItem(m_calculator->factorValueFormatted(timeFctUid));
    itemTime->setEditable(false);
    auto *itemAmpl = new QStandardItem(m_calculator->factorValueFormatted(amplFctUid));
    itemAmpl->setEditable(false);
    auto *itemTryes = new QStandardItem(QString::number(m_calculator->count(stage)));
    itemTryes->setEditable(false);
    auto *itemErrors = new QStandardItem(m_calculator->factorValueFormatted(errFctUid));
    itemErrors->setEditable(false);

    return QList<QStandardItem*>() << itemName << itemLat << itemTime << itemAmpl << itemTryes << itemErrors;
}
