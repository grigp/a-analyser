#include "stepdeviationparamsdialog.h"
#include "ui_stepdeviationparamsdialog.h"

#include "basedefines.h"

StepDeviationParamsDialog::StepDeviationParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepDeviationParamsDialog)
{
    ui->setupUi(this);

    ui->cbDirection->addItems(QStringList()
                           << tr("Вперед")
                           << tr("Вправо")
                           << tr("Назад")
                           << tr("Влево"));
}

StepDeviationParamsDialog::~StepDeviationParamsDialog()
{
    delete ui;
}

void StepDeviationParamsDialog::setParams(const QJsonObject &params)
{
    ui->edMaxTime->setValue(params["max_time"].toInt());

    auto d = params["direction"].toString();
    ui->cbDirection->setCurrentIndex(BaseDefines::DirectionValueIndex.value(d));
}

QJsonObject StepDeviationParamsDialog::getParams()
{
    QJsonObject retval;
    retval["max_time"] = ui->edMaxTime->value();

    auto valD = static_cast<BaseDefines::Directions>(ui->cbDirection->currentIndex());
    retval["direction"] = BaseDefines::DirectionValueUIDName.value(valD);

    return retval;
}
