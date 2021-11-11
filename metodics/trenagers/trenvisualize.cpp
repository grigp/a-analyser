#include "trenvisualize.h"
#include "ui_trenvisualize.h"

#include "aanalyserapplication.h"
#include "trentestcalculator.h"
#include "trenresultfactors.h"
#include "factorsfactory.h"
#include "baseutils.h"
#include "reportelements.h"

namespace
{
QStandardItemModel *mdlFactors;

}

TrenVisualize::TrenVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenVisualize)
{
    ui->setupUi(this);
}

TrenVisualize::~TrenVisualize()
{
    if (m_calculator)
        delete m_calculator;
    delete ui;
}

void TrenVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
        m_calculator = new TrenTestCalculator(testUid);

    m_calculator->calculate();

    auto model = new QStandardItemModel(ui->tvFactorsTrenResult);
    for (int i = 0; i < m_calculator->factorsCount(); ++i)
    {
        auto factorUid = m_calculator->factor(i).uid();
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factorUid);
        auto itemName = new QStandardItem(fi.name());
        itemName->setEditable(false);
        QString sval = "";
        //! Время обрабатываем отдельно
        if (m_calculator->factor(i).uid() == TrenResultFactorsDefines::TimeUid)
            sval = BaseUtils::getTimeBySecCount(static_cast<int>(m_calculator->factor(i).value()));
        else
            sval = QString::number(m_calculator->factor(i).value());
        auto itemValue = new QStandardItem(sval);
        itemValue->setEditable(false);

        model->appendRow(QList<QStandardItem*>() << itemName << itemValue);
    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Значение"));
    ui->tvFactorsTrenResult->setModel(model);
    ui->tvFactorsTrenResult->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    mdlFactors = static_cast<QStandardItemModel*>(ui->tvFactorsTrenResult->model());
}

void TrenVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 5,
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, mdlFactors,
                                  rectTable, QList<int>() << 3 << 1, ReportElements::Table::tvsCompressed,
                                  16, -1, QFont::Bold);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 6),
                        paper.width() / 10 * 8,
                        paper.height() / 10 * 8);
        ReportElements::drawTable(painter, mdlFactors, rectTable,
                                  QList<int>() << 3 << 1, ReportElements::Table::tvsCompressed,
                                  16, -1, QFont::Bold);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}
