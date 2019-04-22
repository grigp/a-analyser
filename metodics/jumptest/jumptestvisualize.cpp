#include "jumptestvisualize.h"
#include "ui_jumptestvisualize.h"

JumpTestVisualize::JumpTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpTestVisualize)
{
    ui->setupUi(this);
}

JumpTestVisualize::~JumpTestVisualize()
{
    delete ui;
}

void JumpTestVisualize::setParams(const QJsonObject &params)
{

}
