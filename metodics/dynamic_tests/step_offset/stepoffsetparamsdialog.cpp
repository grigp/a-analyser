#include "stepoffsetparamsdialog.h"
#include "ui_stepoffsetparamsdialog.h"

#include "baseutils.h"

StepOffsetParamsDialog::StepOffsetParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepOffsetParamsDialog)
{
    ui->setupUi(this);

    ui->cbDirection->addItems(QStringList()
                           << tr("Вперед")
                           << tr("Вправо")
                           << tr("Назад")
                           << tr("Влево"));
}

StepOffsetParamsDialog::~StepOffsetParamsDialog()
{
    delete ui;
}

void StepOffsetParamsDialog::setParams(const QJsonObject &params)
{
    ui->edStageTime->setValue(params["stage_time"].toInt());
    ui->edRepeatCount->setValue(params["repeat_count"].toInt());
    ui->edForce->setValue(params["force"].toInt());

    auto d = params["direction"].toString();
    ui->cbDirection->setCurrentIndex(BaseUtils::DirectionValueIndex.value(d));
}

QJsonObject StepOffsetParamsDialog::getParams()
{
    QJsonObject retval;
    retval["stage_time"] = ui->edStageTime->value();
    retval["repeat_count"] = ui->edRepeatCount->value();
    retval["force"] = ui->edForce->value();

    auto valD = static_cast<BaseUtils::Directions>(ui->cbDirection->currentIndex());
    retval["direction"] = BaseUtils::DirectionValueName.value(valD);

    return retval;
}
