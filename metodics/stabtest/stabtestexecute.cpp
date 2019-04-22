#include "stabtestexecute.h"
#include "ui_stabtestexecute.h"

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
