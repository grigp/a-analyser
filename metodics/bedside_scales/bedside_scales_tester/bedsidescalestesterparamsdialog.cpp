#include "bedsidescalestesterparamsdialog.h"
#include "ui_bedsidescalestesterparamsdialog.h"

BedsideScalesTesterParamsDialog::BedsideScalesTesterParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BedsideScalesTesterParamsDialog)
{
    ui->setupUi(this);
}

BedsideScalesTesterParamsDialog::~BedsideScalesTesterParamsDialog()
{
    delete ui;
}

void BedsideScalesTesterParamsDialog::setParams(const QJsonObject &params)
{

}

QJsonObject BedsideScalesTesterParamsDialog::getParams()
{

}
