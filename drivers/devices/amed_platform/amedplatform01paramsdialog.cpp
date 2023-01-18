#include "amedplatform01paramsdialog.h"
#include "ui_amedplatform01paramsdialog.h"

AMedPlatform01ParamsDialog::AMedPlatform01ParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AMedPlatform01ParamsDialog)
{
    ui->setupUi(this);

    ui->tabPages->setCurrentIndex(0);
    foreach (auto tdKey, DeviceProtocols::tensoDevices.keys())
    {
        ui->cbTensoChan1->addItem(DeviceProtocols::tensoDevices.value(tdKey), tdKey);
        ui->cbTensoChan2->addItem(DeviceProtocols::tensoDevices.value(tdKey), tdKey);
        ui->cbTensoChan3->addItem(DeviceProtocols::tensoDevices.value(tdKey), tdKey);
    }
}

AMedPlatform01ParamsDialog::~AMedPlatform01ParamsDialog()
{
    delete ui;
}

DeviceProtocols::TensoDevice AMedPlatform01ParamsDialog::kindTenso1() const
{
    return static_cast<DeviceProtocols::TensoDevice>(ui->cbTensoChan1->currentData().toInt());
}

void AMedPlatform01ParamsDialog::setKindTenso1(const DeviceProtocols::TensoDevice kind)
{
    auto name = DeviceProtocols::tensoDevices.value(kind);
    ui->cbTensoChan1->setCurrentText(name);
}

DeviceProtocols::TensoDevice AMedPlatform01ParamsDialog::kindTenso2() const
{
    return static_cast<DeviceProtocols::TensoDevice>(ui->cbTensoChan2->currentData().toInt());
}

void AMedPlatform01ParamsDialog::setKindTenso2(const DeviceProtocols::TensoDevice kind)
{
    auto name = DeviceProtocols::tensoDevices.value(kind);
    ui->cbTensoChan2->setCurrentText(name);
}

DeviceProtocols::TensoDevice AMedPlatform01ParamsDialog::kindTenso3() const
{
    return static_cast<DeviceProtocols::TensoDevice>(ui->cbTensoChan2->currentData().toInt());
}

void AMedPlatform01ParamsDialog::setKindTenso3(const DeviceProtocols::TensoDevice kind)
{
    auto name = DeviceProtocols::tensoDevices.value(kind);
    ui->cbTensoChan2->setCurrentText(name);
}

double AMedPlatform01ParamsDialog::rkpTenso1() const
{
    return ui->edRKP1->value();
}

void AMedPlatform01ParamsDialog::setRkpTenso1(const double rkp)
{
    ui->edRKP1->setValue(rkp);
}

double AMedPlatform01ParamsDialog::rkpTenso2() const
{
    return ui->edRKP2->value();
}

void AMedPlatform01ParamsDialog::setRkpTenso2(const double rkp)
{
    ui->edRKP2->setValue(rkp);
}

double AMedPlatform01ParamsDialog::rkpTenso3() const
{
    return ui->edRKP3->value();
}

void AMedPlatform01ParamsDialog::setRkpTenso3(const double rkp)
{
    ui->edRKP3->setValue(rkp);
}

double AMedPlatform01ParamsDialog::pnTenso1() const
{
    return ui->edPn1->value();
}

void AMedPlatform01ParamsDialog::setPnTenso1(const double pn)
{
    ui->edPn1->setValue(static_cast<int>(pn));
}

double AMedPlatform01ParamsDialog::pnTenso2() const
{
    return ui->edPn2->value();
}

void AMedPlatform01ParamsDialog::setPnTenso2(const double pn)
{
    ui->edPn2->setValue(static_cast<int>(pn));
}

double AMedPlatform01ParamsDialog::pnTenso3() const
{
    return ui->edPn3->value();
}

void AMedPlatform01ParamsDialog::setPnTenso3(const double pn)
{
    ui->edPn3->setValue(static_cast<int>(pn));
}
