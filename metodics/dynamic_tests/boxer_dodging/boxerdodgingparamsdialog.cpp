#include "boxerdodgingparamsdialog.h"
#include "ui_boxerdodgingparamsdialog.h"

BoxerDodgingParamsDialog::BoxerDodgingParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoxerDodgingParamsDialog)
{
    ui->setupUi(this);
}

BoxerDodgingParamsDialog::~BoxerDodgingParamsDialog()
{
    delete ui;
}

void BoxerDodgingParamsDialog::setParams(const QJsonObject &params)
{

}

QJsonObject BoxerDodgingParamsDialog::getParams()
{

}
