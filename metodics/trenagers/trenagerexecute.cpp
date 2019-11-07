#include "trenagerexecute.h"
#include "ui_trenagerexecute.h"

TrenagerExecute::TrenagerExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenagerExecute)
{
    ui->setupUi(this);
}

TrenagerExecute::~TrenagerExecute()
{
    delete ui;
}

void TrenagerExecute::setParams(const QJsonObject &params)
{

}
