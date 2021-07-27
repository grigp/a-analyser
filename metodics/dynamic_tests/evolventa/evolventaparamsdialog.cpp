#include "evolventaparamsdialog.h"
#include "ui_evolventaparamsdialog.h"

#include "baseutils.h"

EvolventaParamsDialog::EvolventaParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EvolventaParamsDialog)
{
    ui->setupUi(this);

    ui->cbDirectionMode->addItems(QStringList()
                           << tr("По часовой стрелке")
                           << tr("Против часовой стрелки"));
}

EvolventaParamsDialog::~EvolventaParamsDialog()
{
    delete ui;
}

void EvolventaParamsDialog::setParams(const QJsonObject &params)
{
    ui->edSpeedMax->setValue(params["max_speed"].toInt());
    ui->edRadiusMax->setValue(params["max_radius"].toInt());
    ui->edCircleCount->setValue(params["circles"].toInt());

    auto dm = params["direction_mode"].toString();
    ui->cbDirectionMode->setCurrentIndex(BaseUtils::DirectionModeValueIndex.value(dm) - 1);
}

QJsonObject EvolventaParamsDialog::getParams()
{
    QJsonObject retval;
    retval["max_speed"] = ui->edSpeedMax->value();
    retval["max_radius"] = ui->edRadiusMax->value();
    retval["circles"] = ui->edCircleCount->value();

    auto valDM = static_cast<BaseUtils::DirectionMode>(ui->cbDirectionMode->currentIndex() + 1);
    retval["direction_mode"] = BaseUtils::DirectionModeValueName.value(valDM);

    return retval;
}
