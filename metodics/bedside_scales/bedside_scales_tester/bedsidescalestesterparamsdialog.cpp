#include "bedsidescalestesterparamsdialog.h"
#include "ui_bedsidescalestesterparamsdialog.h"

#include "bedsidescalesdefines.h"


BedsideScalesTesterParamsDialog::BedsideScalesTesterParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BedsideScalesTesterParamsDialog)
{
    ui->setupUi(this);

    ui->cbMode->addItem(BedsideScalesDefines::modeName.value(BedsideScalesDefines::bsmTester), BedsideScalesDefines::bsmTester);
    ui->cbMode->addItem(BedsideScalesDefines::modeName.value(BedsideScalesDefines::bsmScales), BedsideScalesDefines::bsmScales);
    ui->cbMode->addItem(BedsideScalesDefines::modeName.value(BedsideScalesDefines::bsmSleepResearch), BedsideScalesDefines::bsmSleepResearch);
}

BedsideScalesTesterParamsDialog::~BedsideScalesTesterParamsDialog()
{
    delete ui;
}

void BedsideScalesTesterParamsDialog::setParams(const QJsonObject &params)
{
    auto sMode = params["mode"].toString();
    auto mode = BedsideScalesDefines::modeCode.value(sMode);
    ui->cbMode->setCurrentText(BedsideScalesDefines::modeName.value(mode));
    
    auto isAutoRecord = params["auto_record"].toBool(false);
    ui->cbAutoRecord->setChecked(isAutoRecord);
    
    auto startTime = params["start_time"].toString("0:00:20");
    ui->edStartTime->setTime(QTime::fromString(startTime, "h:mm:ss"));

    auto recLength = params["rec_length"].toString("0:10:00");
    ui->edRecLength->setTime(QTime::fromString(recLength, "h:mm:ss"));

    auto isSound = params["sound"].toBool(true);
    ui->cbSound->setChecked(isSound);

    auto isIntervalScaling = params["is_interval_scaling"].toBool(true);
    ui->cbIntervalScaling->setChecked(isIntervalScaling);

    auto scalingInterval = params["scaling_interval"].toString("05:00");
    ui->edScalingInterval->setTime(QTime::fromString(scalingInterval, "mm:ss"));

    auto averageTime = params["average_time"].toInt(3);
    ui->edAverageTime->setValue(averageTime);
}

QJsonObject BedsideScalesTesterParamsDialog::getParams()
{
    QJsonObject retval;

    auto code = static_cast<BedsideScalesDefines::Mode>(ui->cbMode->currentData().toInt());
    auto val = BedsideScalesDefines::modeUid.value(code);
    retval["mode"] = val;

    retval["auto_record"] = ui->cbAutoRecord->isChecked();
    retval["start_time"] = ui->edStartTime->time().toString("h:mm:ss");
    retval["rec_length"] = ui->edRecLength->time().toString("h:mm:ss");
    retval["sound"] = ui->cbSound->isChecked();

    retval["is_interval_scaling"] = ui->cbIntervalScaling->isChecked();
    retval["scaling_interval"] = ui->edScalingInterval->time().toString("mm:ss");
    retval["average_time"] = ui->edAverageTime->value();

    return retval;
}

void BedsideScalesTesterParamsDialog::on_modeChanged(int mode)
{
    Q_UNUSED(mode);
    ui->frScalesMode->setEnabled(ui->cbMode->currentData().toInt() == BedsideScalesDefines::bsmScales);
}

