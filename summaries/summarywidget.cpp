#include "summarywidget.h"
#include "ui_summarywidget.h"

#include "summary.h"
#include "summaryheaderdelegate.h"

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
    ui->tvSummary->setItemDelegateForColumn(0, new SummaryHeaderDelegate(ui->tvSummary));
    ui->tvSummary->setItemDelegateForRow(0, new SummaryHeaderDelegate(ui->tvSummary));
    ui->tvSummary->setItemDelegateForRow(1, new SummaryHeaderDelegate(ui->tvSummary));
    ui->tvSummary->setItemDelegateForRow(2, new SummaryHeaderDelegate(ui->tvSummary));
    ui->tvSummary->setItemDelegateForRow(3, new SummaryHeaderDelegate(ui->tvSummary));

    for (int i = 1; i < model->columnCount(); ++i)
        ui->tvSummary->horizontalHeader()->resizeSection(i, 60);
    ui->tvSummary->horizontalHeader()->resizeSection(0, 250);
    ui->tvSummary->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);

    for (int i = 0; i < model->spanCellsCount(); ++i)
    {
        auto sc = model->spanCells(i);
        if (sc.width > 0)
            ui->tvSummary->setSpan(sc.row, sc.col, 1, sc.width);
    }
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
    auto showValue = [&](const int row, const int role, QLabel* label, const QString staticText)
    {
        QString mfn = "";
        int i = index.column();
        do
        {
            mfn = model()->index(row, i, index.parent()).data(role).toString();
            --i;
        }
        while (i >= 0 && mfn == "");
        if (mfn != "")
            label->setText(staticText + " - " + mfn);
        else
            label->setText("");
    };

    //! Вывод названия показателя
    auto fn = model()->index(Summary::srFactors, index.column(), index.parent()).data(Summary::FactorNameRole).toString();
    if (fn != "")
        ui->lblFactorName->setText(tr("Показатель") + " - " + fn);
    else
        ui->lblFactorName->setText("");

    //! Вывод названия группы показателей
    showValue(Summary::srMultifactors, Summary::MultiFactorNameRole, ui->lblMultiFactorName, tr("Группа показателей"));

    //! Вывод названия канала
    showValue(Summary::srChannels, Summary::ChannelNameRole, ui->lblChannelName, tr("Канал"));

    //! Вывод названия пробы
    showValue(Summary::srProbes, Summary::ProbeNameRole, ui->lblProbeName, tr("Проба"));

    //! Вывод названия методики
    showValue(Summary::srProbes, Summary::MethodicNameRole, ui->lblMethodName, tr("Методика"));
}
