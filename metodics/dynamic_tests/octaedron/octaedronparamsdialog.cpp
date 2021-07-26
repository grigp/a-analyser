#include "octaedronparamsdialog.h"
#include "ui_octaedronparamsdialog.h"

OctaedronParamsDialog::OctaedronParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OctaedronParamsDialog)
{
    ui->setupUi(this);

    ui->cbDirectionMode->addItems(QStringList()
                           << tr("Случайный")
                           << tr("По часовой стрелке")
                           << tr("Против часовой стрелки"));
    ui->cbCirceRoundRuleMode->addItems(QStringList()
                              << tr("Радиальное")
                              << tr("Кольцевое"));
}

OctaedronParamsDialog::~OctaedronParamsDialog()
{
    delete ui;
}

void OctaedronParamsDialog::setParams(const QJsonObject &params)
{

}

QJsonObject OctaedronParamsDialog::getParams()
{

}
