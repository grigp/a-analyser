#include "amedplatform01paramsdialog.h"
#include "ui_amedplatform01paramsdialog.h"

AMedPlatform01ParamsDialog::AMedPlatform01ParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AMedPlatform01ParamsDialog)
{
    ui->setupUi(this);
}

AMedPlatform01ParamsDialog::~AMedPlatform01ParamsDialog()
{
    delete ui;
}

DeviceProtocols::TensoDevice AMedPlatform01ParamsDialog::kindTenso1() const
{

}

void AMedPlatform01ParamsDialog::setKindTenso1(const DeviceProtocols::TensoDevice kind)
{

}

DeviceProtocols::TensoDevice AMedPlatform01ParamsDialog::kindTenso2() const
{

}

void AMedPlatform01ParamsDialog::setKindTenso2(const DeviceProtocols::TensoDevice kind)
{

}

DeviceProtocols::TensoDevice AMedPlatform01ParamsDialog::kindTenso3() const
{

}

void AMedPlatform01ParamsDialog::setKindTenso3(const DeviceProtocols::TensoDevice kind)
{

}

double AMedPlatform01ParamsDialog::rkpTenso1() const
{

}

void AMedPlatform01ParamsDialog::setRkpTenso1(const double rkp)
{

}

double AMedPlatform01ParamsDialog::rkpTenso2() const
{

}

void AMedPlatform01ParamsDialog::setRkpTenso2(const double rkp)
{

}

double AMedPlatform01ParamsDialog::rkpTenso3() const
{

}

void AMedPlatform01ParamsDialog::setRkpTenso3(const double rkp)
{

}

double AMedPlatform01ParamsDialog::pnTenso1() const
{

}

void AMedPlatform01ParamsDialog::setPnTenso1(const double pn)
{

}

double AMedPlatform01ParamsDialog::pnTenso2() const
{

}

void AMedPlatform01ParamsDialog::setPnTenso2(const double pn)
{

}

double AMedPlatform01ParamsDialog::pnTenso3() const
{

}

void AMedPlatform01ParamsDialog::setPnTenso3(const double pn)
{

}
