#include "summarieswidget.h"
#include "ui_summarieswidget.h"

#include <QStandardItemModel>
#include <QTableView>
#include <QDebug>

#include "aanalyserapplication.h"
#include "summary.h"
#include "summarywidget.h"


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
        auto tv = new SummaryWidget(ui->frSummaries);
        tv->setModel(summary);
        ui->frSummaries->layout()->addWidget(tv);

        for (int i = 0; i < summary->spanCellsCount(); ++i)
        {
            auto sc = summary->spanCells(i);
            if (sc.width > 0)
                tv->setSpan(sc.row, sc.col, sc.width);

        }

        static_cast<AAnalyserApplication*>(QApplication::instance())->summaries();
    }
}

