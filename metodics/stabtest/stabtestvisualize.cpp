#include "stabtestvisualize.h"
#include "ui_stabtestvisualize.h"

StabTestVisualize::StabTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabTestVisualize)
{
    ui->setupUi(this);
}

StabTestVisualize::~StabTestVisualize()
{
    delete ui;
}

void StabTestVisualize::setParams(const QJsonObject &params)
{

}
