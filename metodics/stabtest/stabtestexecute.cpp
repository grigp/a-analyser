#include "stabtestexecute.h"
#include "ui_stabtestexecute.h"

#include "executewidget.h"

StabTestExecute::StabTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabTestExecute)
{
    ui->setupUi(this);
}

StabTestExecute::~StabTestExecute()
{
    delete ui;
}

void StabTestExecute::setParams(const QJsonObject &params)
{

}

void StabTestExecute::showDataBase()
{
    static_cast<ExecuteWidget*>(parent())->showDB();
}
