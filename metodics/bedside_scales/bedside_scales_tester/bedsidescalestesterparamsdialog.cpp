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
}

QJsonObject BedsideScalesTesterParamsDialog::getParams()
{
    QJsonObject retval;

    auto code = static_cast<BedsideScalesDefines::Mode>(ui->cbMode->currentData().toInt());
    auto val = BedsideScalesDefines::modeUid.value(code);
    retval["mode"] = val;

    return retval;
}

