#include "octaedronparamsdialog.h"
#include "ui_octaedronparamsdialog.h"

#include "aanalyserapplication.h"
#include "basedefines.h"
#include "amessagebox.h"

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
    ui->cbStageFinishMode->addItems(QStringList()
                                    << tr("Через заданное время")
                                    << tr("При достижении цели"));
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
    auto sfm = params["stage_fixing_mode"].toString();
    ui->cbStageFinishMode->setCurrentIndex(BaseDefines::StageFinishModeValueIndex.value(sfm));

    ui->edStageTime->setValue(params["stage_time"].toInt());
    ui->edRadius->setValue(params["radius"].toInt());
    ui->edHoldingTime->setValue(params["holding_time"].toInt(1));
    ui->edHoldingAmplitude->setValue(params["holding_amplitude"].toInt(10));
    ui->cbShowOnlyCurrentPoint->setChecked(params["only_current_point"].toBool(false));
}

QJsonObject OctaedronParamsDialog::getParams()
{
    QJsonObject retval;
    auto valDM = static_cast<BaseDefines::DirectionMode>(ui->cbDirectionMode->currentIndex());
    retval["direction_mode"] = BaseDefines::DirectionModeValueName.value(valDM);
    auto valCRM = static_cast<BaseDefines::CirceRoundRuleMode>(ui->cbCirceRoundRuleMode->currentIndex());
    retval["circe_round_rule_mode"] = BaseDefines::CirceRoundRuleModeValueName.value(valCRM);
    auto valSFM = static_cast<BaseDefines::StageFinishMode>(ui->cbStageFinishMode->currentIndex());
    retval["stage_fixing_mode"] = BaseDefines::StageFinishModeValueName.value(valSFM);

    retval["stage_time"] = ui->edStageTime->value();
    retval["radius"] = ui->edRadius->value();
    retval["holding_time"] = ui->edHoldingTime->value();
    retval["holding_amplitude"] = ui->edHoldingAmplitude->value();
    retval["only_current_point"] = ui->cbShowOnlyCurrentPoint->isChecked();

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

void OctaedronParamsDialog::on_StageFinishMode(int idx)
{
    ui->frHoldingTime->setEnabled(idx == 1);
    ui->frHoldingAmplitude->setEnabled(idx == 1);
}

void OctaedronParamsDialog::on_ok()
{
    accept();
}

void OctaedronParamsDialog::on_cancel()
{
    reject();
}

void OctaedronParamsDialog::on_default()
{
    auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (AMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}
