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
    ui->edDeviationThreshold->setValue(params["deviation_threshold"].toInt());
    ui->edTime->setValue(params["time"].toInt());
    ui->edStimulTimeMin->setValue(params["min_len"].toInt());
    ui->edStimulTimeMax->setValue(params["max_len"].toInt());
}

QJsonObject BoxerDodgingParamsDialog::getParams()
{
    QJsonObject retval;

    retval["deviation_threshold"] = ui->edDeviationThreshold->value();
    retval["time"] = ui->edTime->value();
    retval["min_len"] = ui->edStimulTimeMin->value();
    retval["max_len"] = ui->edStimulTimeMax->value();

    return retval;
}
