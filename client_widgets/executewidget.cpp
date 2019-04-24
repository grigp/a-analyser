#include "executewidget.h"
#include "ui_executewidget.h"

#include <QApplication>
#include "aanalyserapplication.h"
#include "databaseresultwidget.h"

#include <QDebug>

ExecuteWidget::ExecuteWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::ExecuteWidget)
{
    ui->setupUi(this);
}

ExecuteWidget::~ExecuteWidget()
{
    delete ui;
}

QString ExecuteWidget::uid()
{
    return ClientWidgets::uidExecuteWidgetUid;
}

QString ExecuteWidget::name()
{
    return tr("Проведение теста");
}

void ExecuteWidget::showDB()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
}
