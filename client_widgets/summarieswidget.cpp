#include "summarieswidget.h"
#include "ui_summarieswidget.h"

#include <QStandardItemModel>
#include <QTableView>
#include <QDebug>

#include "aanalyserapplication.h"
#include "summary.h"


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
