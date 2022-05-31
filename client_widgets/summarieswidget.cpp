#include "summarieswidget.h"
#include "ui_summarieswidget.h"

#include <QStandardItemModel>
#include <QTableView>
#include <QDebug>

#include "aanalyserapplication.h"
#include "summary.h"
#include "summatyheaderdelegate.h"


SummariesWidget::SummariesWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::SummariesWidget)
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::addTestToSummary,
            this, &SummariesWidget::addTestToSummary);
}

SummariesWidget::~SummariesWidget()
{
    delete ui;
}

QString SummariesWidget::uid()
{
    return ClientWidgets::uidSummariesWidgetUid;
}

QString SummariesWidget::name()
{
    return tr("Сводки показателей");
}

void SummariesWidget::onShow()
{
    testTable();
}

void SummariesWidget::onHide()
{

}

void SummariesWidget::addTestToSummary(const QString testUid,
                                       const SummaryDefines::AddTestMode mode,
                                       const QString summaryName,
                                       const SummaryDefines::Kind kind)
{
    if (mode == SummaryDefines::atmNew)
    {
        auto summary = new Summary();
        summary->setName(summaryName);
        summary->setKind(kind);
        summary->addTest(testUid);
        auto tv = new QTableView(ui->frSummaries);
        tv->setModel(summary);
        ui->frSummaries->layout()->addWidget(tv);

        for (int i = 0; i < summary->spanCellsCount(); ++i)
        {
            auto sc = summary->spanCells(i);
            if (sc.width > 0)
                tv->setSpan(sc.row, sc.col, 1, sc.width);

        }
//        tv->horizontalHeader()->setVisible(false);
//        tv->verticalHeader()->setVisible(false);
        tv->setItemDelegateForColumn(0, new SummatyHeaderDelegate(tv));
        tv->setItemDelegateForRow(0, new SummatyHeaderDelegate(tv));
        tv->setItemDelegateForRow(1, new SummatyHeaderDelegate(tv));
        tv->setItemDelegateForRow(2, new SummatyHeaderDelegate(tv));
        tv->setItemDelegateForRow(3, new SummatyHeaderDelegate(tv));

        for (int i = 1; i < summary->columnCount(); ++i)
            tv->horizontalHeader()->resizeSection(i, 60);
        tv->horizontalHeader()->resizeSection(0, 250);
        tv->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);

        static_cast<AAnalyserApplication*>(QApplication::instance())->summaries();
    }
}

void SummariesWidget::testTable()
{
//    auto model = new QStandardItemModel(ui->summary);

//    QList<QStandardItem*> items;

//    for (int l = 0; l < 20; ++l)
//    {
//        items.clear();
//        for (int i = 0; i < 10; ++i)
//        {
//            items << new QStandardItem(QString::number(l+1) + " " + QString::number(i+1) + " : " + QString::number(qrand() % 1000));
//        }
//        model->appendRow(items);
//    }

//    ui->summary->setModel(model);

//    ui->summary->horizontalHeader()->setVisible(false);
//    ui->summary->verticalHeader()->setVisible(false);
//    ui->summary->setSpan(0,0, 2, 2);
}
