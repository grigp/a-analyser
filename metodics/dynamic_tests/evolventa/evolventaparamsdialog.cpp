#include "evolventaparamsdialog.h"
#include "ui_evolventaparamsdialog.h"

#include "aanalyserapplication.h"
#include "basedefines.h"

#include <QMessageBox>

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
    ui->cbDirectionMode->setCurrentIndex(BaseDefines::DirectionModeValueIndex.value(dm) - 1);
}

QJsonObject EvolventaParamsDialog::getParams()
{
    QJsonObject retval;
    retval["max_speed"] = ui->edSpeedMax->value();
    retval["max_radius"] = ui->edRadiusMax->value();
    retval["circles"] = ui->edCircleCount->value();

    auto valDM = static_cast<BaseDefines::DirectionMode>(ui->cbDirectionMode->currentIndex() + 1);
    retval["direction_mode"] = BaseDefines::DirectionModeValueName.value(valDM);

    return retval;
}

void EvolventaParamsDialog::on_ok()
{
    accept();
}

void EvolventaParamsDialog::on_cancel()
{
    reject();
}

void EvolventaParamsDialog::on_default()
{
    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (QMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}
