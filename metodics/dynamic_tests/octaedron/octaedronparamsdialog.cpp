#include "octaedronparamsdialog.h"
#include "ui_octaedronparamsdialog.h"

#include "baseutils.h"

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
    ui->cbCirceRoundRuleMode->setCurrentIndex(BaseUtils::CirceRoundRuleModeValueIndex.value(crm));
    auto dm = params["direction_mode"].toString();
    ui->cbDirectionMode->setCurrentIndex(BaseUtils::DirectionModeValueIndex.value(dm));

    ui->edStageTime->setValue(params["stage_time"].toInt());
    ui->edRadius->setValue(params["radius"].toInt());
}

QJsonObject OctaedronParamsDialog::getParams()
{
    QJsonObject retval;
    auto valDM = static_cast<BaseUtils::DirectionMode>(ui->cbDirectionMode->currentIndex());
    retval["direction_mode"] = BaseUtils::DirectionModeValueName.value(valDM);
    auto valCRM = static_cast<BaseUtils::CirceRoundRuleMode>(ui->cbCirceRoundRuleMode->currentIndex());
    retval["circe_round_rule_mode"] = BaseUtils::CirceRoundRuleModeValueName.value(valCRM);

    retval["stage_time"] = ui->edStageTime->value();
    retval["radius"] = ui->edRadius->value();

    return retval;
}

void OctaedronParamsDialog::onCirceRoundRuleMode(int idx)
{
    if (idx == BaseUtils::crmCircle)
        if (ui->cbDirectionMode->currentIndex() == BaseUtils::dmRandom)
            ui->cbDirectionMode->setCurrentIndex(BaseUtils::dmCounterClockwise);
}

void OctaedronParamsDialog::onDirectionModeChange(int idx)
{
    if (idx == BaseUtils::dmRandom)
        if (ui->cbCirceRoundRuleMode->currentIndex() == BaseUtils::crmCircle)
            ui->cbCirceRoundRuleMode->setCurrentIndex(BaseUtils::crmRadial);
}
