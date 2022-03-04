#include "jumpplateparamsdialog.h"
#include "ui_jumpplateparamsdialog.h"

JumpPlateParamsDialog::JumpPlateParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumpPlateParamsDialog)
{
    ui->setupUi(this);

    ui->cbPlateCount->addItem("1", 1);
    ui->cbPlateCount->addItem("2", 2);
}

JumpPlateParamsDialog::~JumpPlateParamsDialog()
{
    delete ui;
}

int JumpPlateParamsDialog::platformsCount() const
{
    return ui->cbPlateCount->currentData().toInt();
}

void JumpPlateParamsDialog::setPlatformsCount(const int count)
{
    auto idx = ui->cbPlateCount->findData(count);
    if (idx > -1)
        ui->cbPlateCount->setCurrentIndex(idx);
}
