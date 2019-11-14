#include "trentakeputexecute.h"
#include "ui_trentakeputexecute.h"

TrenTakePutExecute::TrenTakePutExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenTakePutExecute)
{
    ui->setupUi(this);
}

TrenTakePutExecute::~TrenTakePutExecute()
{
    delete ui;
}

void TrenTakePutExecute::setParams(const QJsonObject &params)
{

}
