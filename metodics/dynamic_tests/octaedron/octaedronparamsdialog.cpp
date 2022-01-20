#include "octaedronparamsdialog.h"
#include "ui_octaedronparamsdialog.h"

#include "basedefines.h"

OctaedronParamsDialog::OctaedronParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OctaedronParamsDialog)
{
    ui->setupUi(this);

    ui->cbDirectionMode->addItems(QStringList()
                           << tr("Случайный")
                           << tr("По часовой стрелке")
                           << tr("Против часовой стрелки"));
    ui->cbCirceRoundRuleMode->addItems(QStringList()
                              << tr("Радиальное")
                              << tr("Кольцевое"));
}

OctaedronParamsDialog::~OctaedronParamsDialog()
{
    delete ui;
}

void OctaedronParamsDialog::setParams(const QJsonObject &params)
{
    auto crm = params["circe_round_rule_mode"].toString();
    ui->cbCirceRoundRuleMode->setCurrentIndex(BaseDefines::CirceRoundRuleModeValueIndex.value(crm));
    auto dm = params["direction_mode"].toString();
    ui->cbDirectionMode->setCurrentIndex(BaseDefines::DirectionModeValueIndex.value(dm));

    ui->edStageTime->setValue(params["stage_time"].toInt());
    ui->edRadius->setValue(params["radius"].toInt());
}

QJsonObject OctaedronParamsDialog::getParams()
{
    QJsonObject retval;
    auto valDM = static_cast<BaseDefines::DirectionMode>(ui->cbDirectionMode->currentIndex());
    retval["direction_mode"] = BaseDefines::DirectionModeValueName.value(valDM);
    auto valCRM = static_cast<BaseDefines::CirceRoundRuleMode>(ui->cbCirceRoundRuleMode->currentIndex());
    retval["circe_round_rule_mode"] = BaseDefines::CirceRoundRuleModeValueName.value(valCRM);

    retval["stage_time"] = ui->edStageTime->value();
    retval["radius"] = ui->edRadius->value();

    return retval;
}

void OctaedronParamsDialog::onCirceRoundRuleMode(int idx)
{
    if (idx == BaseDefines::crmCircle)
        if (ui->cbDirectionMode->currentIndex() == BaseDefines::dmRandom)
            ui->cbDirectionMode->setCurrentIndex(BaseDefines::dmCounterClockwise);
}

void OctaedronParamsDialog::onDirectionModeChange(int idx)
{
    if (idx == BaseDefines::dmRandom)
        if (ui->cbCirceRoundRuleMode->currentIndex() == BaseDefines::crmCircle)
            ui->cbCirceRoundRuleMode->setCurrentIndex(BaseDefines::crmRadial);
}
