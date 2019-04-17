#include "databaseresultwidget.h"
#include "ui_databaseresultwidget.h"

#include <QApplication>
#include "aanalyserapplication.h"
#include "executewidget.h"

DataBaseResultWidget::DataBaseResultWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::DataBaseResultWidget)
{
    ui->setupUi(this);
}

DataBaseResultWidget::~DataBaseResultWidget()
{
    delete ui;
}

QString DataBaseResultWidget::uid()
{
    return ClientWidgets::uidDatabaseResultWidgetUid;;
}

QString DataBaseResultWidget::name()
{
    return tr("База данных");
}

void DataBaseResultWidget::executeProbe()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidExecuteWidgetUid);
}
