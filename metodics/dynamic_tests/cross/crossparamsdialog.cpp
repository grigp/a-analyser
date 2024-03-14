#include "crossparamsdialog.h"
#include "ui_crossparamsdialog.h"

#include "aanalyserapplication.h"
#include "crossdefines.h"
#include "basedefines.h"

#include <QMessageBox>
#include <QDebug>

CrossParamsDialog::CrossParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CrossParamsDialog)
{
    ui->setupUi(this);

    ui->cbDirectionMode->addItems(QStringList()
                           << tr("Случайный")
                           << tr("По часовой стрелке")
                           << tr("Против часовой стрелки"));
    ui->cbChangeStateMode->addItems(QStringList()
                           << tr("возврата в центр")
                           << tr("заданное время"));
}

CrossParamsDialog::~CrossParamsDialog()
{
    delete ui;
}

void CrossParamsDialog::setParams(const QJsonObject &params)
{
    ui->edStageTime->setValue(params["stage_time"].toInt());
    ui->edRepeatCount->setValue(params["repeat_count"].toInt());
    ui->edCenterSize->setValue(params["center_size"].toInt());
    ui->edDelayTime->setValue(params["delay_time"].toInt());

    auto csm = params["change_stage_mode"].toString();
    ui->cbChangeStateMode->setCurrentIndex(CrossDefines::ChangeStateModeValueIndex.value(csm));
    auto dm = params["direction_mode"].toString();
    ui->cbDirectionMode->setCurrentIndex(BaseDefines::DirectionModeValueIndex.value(dm));
}

QJsonObject CrossParamsDialog::getParams()
{
    QJsonObject retval;
    retval["stage_time"] = ui->edStageTime->value();
    retval["repeat_count"] = ui->edRepeatCount->value();
    retval["center_size"] = ui->edCenterSize->value();
    retval["delay_time"] = ui->edDelayTime->value();

    auto valCSM = static_cast<CrossDefines::ChangeStateMode>(ui->cbChangeStateMode->currentIndex());
    retval["change_stage_mode"] = CrossDefines::ChangeStateModeValueName.value(valCSM);
    auto valDM = static_cast<BaseDefines::DirectionMode>(ui->cbDirectionMode->currentIndex());
    retval["direction_mode"] = BaseDefines::DirectionModeValueName.value(valDM);

    return retval;
}

void CrossParamsDialog::onChangeStateMode(int index)
{
    ui->frCenterSize->setVisible(index == 0);
    ui->frDelayTime->setVisible(index == 1);
}

void CrossParamsDialog::on_ok()
{
    accept();
}

void CrossParamsDialog::on_cancel()
{
    reject();
}

void CrossParamsDialog::on_default()
{
    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (QMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}
