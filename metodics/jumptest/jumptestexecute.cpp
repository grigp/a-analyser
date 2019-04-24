#include "jumptestexecute.h"
#include "ui_jumptestexecute.h"

#include "executewidget.h"

JumpTestExecute::JumpTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpTestExecute)
{
    ui->setupUi(this);
}

JumpTestExecute::~JumpTestExecute()
{
    delete ui;
}

void JumpTestExecute::setParams(const QJsonObject &params)
{

}

void JumpTestExecute::showDataBase()
{
    static_cast<ExecuteWidget*>(parent())->showDB();
}
