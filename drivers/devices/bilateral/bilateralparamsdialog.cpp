#include "bilateralparamsdialog.h"
#include "ui_bilateralparamsdialog.h"

BilateralParamsDialog::BilateralParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BilateralParamsDialog)
{
    ui->setupUi(this);

}

BilateralParamsDialog::~BilateralParamsDialog()
{
    delete ui;
}

int BilateralParamsDialog::exec()
{
    ui->frInfo->setStyleSheet("background-color: rgb(205, 220, 230);");

    connect(ui->wgtDiag, &PlatesPositioning::movePlate, this, &BilateralParamsDialog::on_movePlate);

    return QDialog::exec();
}

QPoint BilateralParamsDialog::getPlate1Pos() const
{
    return QPoint(ui->edStab1KoordX->value(), ui->edStab1KoordY->value());
}

void BilateralParamsDialog::setPlate1Pos(const QPoint &pos)
{
    ui->edStab1KoordX->setValue(pos.x());
    ui->edStab1KoordY->setValue(pos.y());
}

QSize BilateralParamsDialog::getPlate1Size() const
{
    return QSize(ui->edStab1SizeW->value(), ui->edStab1SizeH->value());
}

void BilateralParamsDialog::setPlate1Size(const QSize &size)
{
    ui->edStab1SizeW->setValue(size.width());
    ui->edStab1SizeH->setValue(size.height());
}

QPoint BilateralParamsDialog::getPlate2Pos() const
{
    return QPoint(ui->edStab2KoordX->value(), ui->edStab2KoordY->value());
}

void BilateralParamsDialog::setPlate2Pos(const QPoint &pos)
{
    ui->edStab2KoordX->setValue(pos.x());
    ui->edStab2KoordY->setValue(pos.y());
}

QSize BilateralParamsDialog::getPlate2Size() const
{
    return QSize(ui->edStab2SizeW->value(), ui->edStab2SizeH->value());
}

void BilateralParamsDialog::setPlate2Size(const QSize &size)
{
    ui->edStab2SizeW->setValue(size.width());
    ui->edStab2SizeH->setValue(size.height());
}

void BilateralParamsDialog::on_movePlate(const int plate, const int x, const int y)
{
    if (plate == 0)
    {
        ui->edStab1KoordX->setValue(x);
        ui->edStab1KoordY->setValue(y);
        ui->wgtDiag->setPlate1(x, y);
    }
    else
    if (plate == 1)
    {
        ui->edStab2KoordX->setValue(x);
        ui->edStab2KoordY->setValue(y);
        ui->wgtDiag->setPlate2(x, y);
    }
}
