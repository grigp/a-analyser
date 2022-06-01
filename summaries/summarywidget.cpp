#include "summarywidget.h"
#include "ui_summarywidget.h"

#include "summary.h"
#include "summatyheaderdelegate.h"

#include <QDebug>

SummaryWidget::SummaryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SummaryWidget)
{
    ui->setupUi(this);
}

SummaryWidget::~SummaryWidget()
{
    delete ui;
}

void SummaryWidget::setModel(Summary *model)
{
    ui->tvSummary->setModel(model);

    //        tv->horizontalHeader()->setVisible(false);
    //        tv->verticalHeader()->setVisible(false);
    ui->tvSummary->setItemDelegateForColumn(0, new SummatyHeaderDelegate(ui->tvSummary));
    ui->tvSummary->setItemDelegateForRow(0, new SummatyHeaderDelegate(ui->tvSummary));
    ui->tvSummary->setItemDelegateForRow(1, new SummatyHeaderDelegate(ui->tvSummary));
    ui->tvSummary->setItemDelegateForRow(2, new SummatyHeaderDelegate(ui->tvSummary));
    ui->tvSummary->setItemDelegateForRow(3, new SummatyHeaderDelegate(ui->tvSummary));

    for (int i = 1; i < model->columnCount(); ++i)
        ui->tvSummary->horizontalHeader()->resizeSection(i, 60);
    ui->tvSummary->horizontalHeader()->resizeSection(0, 250);
    ui->tvSummary->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

Summary *SummaryWidget::model() const
{
    return static_cast<Summary*>(ui->tvSummary->model());
}

void SummaryWidget::setSpan(const int row, const int col, const int columnSpan)
{
    ui->tvSummary->setSpan(row, col, 1, columnSpan);
}

void SummaryWidget::on_ItemSelected(QModelIndex index)
{
    auto fn = model()->index(Summary::srFactors, index.column(), index.parent()).data(Summary::FactorNameRole).toString();
    ui->lblFactorName->setText(tr("Показатель") + " - " + fn);

}
