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

    return retval;
}

