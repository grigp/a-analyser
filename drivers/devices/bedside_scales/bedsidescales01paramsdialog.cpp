#include "bedsidescales01paramsdialog.h"
#include "ui_bedsidescales01paramsdialog.h"

BedsideScales01ParamsDialog::BedsideScales01ParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BedsideScales01ParamsDialog)
{
    ui->setupUi(this);
}

BedsideScales01ParamsDialog::~BedsideScales01ParamsDialog()
{
    delete ui;
}

double BedsideScales01ParamsDialog::rkpSensor1() const
{
    return ui->edSensor1RKP->value();
}

void BedsideScales01ParamsDialog::setRkpSensor1(const double rkp)
{
    ui->edSensor1RKP->setValue(rkp);
}

double BedsideScales01ParamsDialog::rkpSensor2() const
{
    return ui->edSensor2RKP->value();
}

void BedsideScales01ParamsDialog::setRkpSensor2(const double rkp)
{
    ui->edSensor2RKP->setValue(rkp);
}

double BedsideScales01ParamsDialog::rkpSensor3() const
{
    return ui->edSensor3RKP->value();
}

void BedsideScales01ParamsDialog::setRkpSensor3(const double rkp)
{
    ui->edSensor3RKP->setValue(rkp);
}

double BedsideScales01ParamsDialog::rkpSensor4() const
{
    return ui->edSensor4RKP->value();
}

void BedsideScales01ParamsDialog::setRkpSensor4(const double rkp)
{
    ui->edSensor4RKP->setValue(rkp);
}

int BedsideScales01ParamsDialog::pn() const
{
    return ui->edPN->value();
}

void BedsideScales01ParamsDialog::setPn(const int pn)
{
    ui->edPN->setValue(pn);
}
