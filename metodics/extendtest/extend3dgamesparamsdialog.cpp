#include "extend3dgamesparamsdialog.h"
#include "ui_extend3dgamesparamsdialog.h"

Extend3DGamesParamsDialog::Extend3DGamesParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Extend3DGamesParamsDialog)
{
    ui->setupUi(this);
}

Extend3DGamesParamsDialog::~Extend3DGamesParamsDialog()
{
    delete ui;
}

void Extend3DGamesParamsDialog::setParams(const QJsonObject &params)
{

}

QJsonObject Extend3DGamesParamsDialog::getParams()
{
    return  QJsonObject();
}
