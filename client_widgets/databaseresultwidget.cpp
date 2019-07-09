#include "databaseresultwidget.h"
#include "ui_databaseresultwidget.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"

#include <QDebug>

DataBaseResultWidget::DataBaseResultWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::DataBaseResultWidget)
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectTest,
            this, &DataBaseResultWidget::onSelectTest);

}

DataBaseResultWidget::~DataBaseResultWidget()
{
    delete ui;
}

QString DataBaseResultWidget::uid()
{
    return ClientWidgets::uidDatabaseResultWidgetUid;
}

QString DataBaseResultWidget::name()
{
    return tr("База данных");
}

void DataBaseResultWidget::onDBConnect()
{
    ui->wgtPatients->onDbConnect();
    ui->wgtMetods->onDbConnect();
    ui->wgtTests->onDbConnect();
}

void DataBaseResultWidget::onDBDisconnect()
{

}

void DataBaseResultWidget::onSelectTest(const QString &testUid)
{
}


