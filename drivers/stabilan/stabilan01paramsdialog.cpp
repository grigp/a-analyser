#include "stabilan01paramsdialog.h"
#include "ui_stabilan01paramsdialog.h"

#include "stabilan01.h"

Stabilan01ParamsDialog::Stabilan01ParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Stabilan01ParamsDialog)
{
    ui->setupUi(this);

    foreach (auto modelCode, Stabilan01::models())
        ui->cbModel->addItem(Stabilan01::modelName(modelCode), modelCode);

    foreach (auto ztCode, Stabilan01::zeroingTypes())
        ui->cbZeroingType->addItem(Stabilan01::zeroingTypeName(ztCode), ztCode);
}

Stabilan01ParamsDialog::~Stabilan01ParamsDialog()
{
    delete ui;
}

Stabilan01Defines::Model Stabilan01ParamsDialog::model() const
{
    return static_cast<Stabilan01Defines::Model>(ui->cbModel->currentData().toInt());
}

void Stabilan01ParamsDialog::setModel(const Stabilan01Defines::Model mdl)
{
    ui->cbModel->setCurrentText(Stabilan01::modelName(mdl));
}

Stabilan01Defines::ZeroingType Stabilan01ParamsDialog::zeroingType() const
{
    return static_cast<Stabilan01Defines::ZeroingType>(ui->cbZeroingType->currentData().toInt());
}

void Stabilan01ParamsDialog::setZeroingType(const Stabilan01Defines::ZeroingType zt)
{
    ui->cbZeroingType->setCurrentText(Stabilan01::zeroingTypeName(zt));
}
