#include "summarieswidget.h"
#include "ui_summarieswidget.h"

#include <QStandardItemModel>
#include <QDebug>


SummariesWidget::SummariesWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::SummariesWidget)
{
    ui->setupUi(this);
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

void SummariesWidget::testTable()
{
    auto model = new QStandardItemModel(ui->summary);

    QList<QStandardItem*> items;

    for (int l = 0; l < 20; ++l)
    {
        items.clear();
        for (int i = 0; i < 10; ++i)
        {
            items << new QStandardItem(QString::number(l+1) + " " + QString::number(i+1) + " : " + QString::number(qrand() % 1000));
        }
        model->appendRow(items);
    }

    ui->summary->setModel(model);

    ui->summary->horizontalHeader()->setVisible(false);
    ui->summary->verticalHeader()->setVisible(false);
    ui->summary->setSpan(0,0, 2, 2);
}
