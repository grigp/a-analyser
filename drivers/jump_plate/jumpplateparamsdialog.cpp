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
