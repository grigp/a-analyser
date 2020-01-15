#include "jumpheighttestvisualize.h"
#include "ui_jumpheighttestvisualize.h"

JumpHeightTestVisualize::JumpHeightTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpHeightTestVisualize)
{
    ui->setupUi(this);
}

JumpHeightTestVisualize::~JumpHeightTestVisualize()
{
    delete ui;
}

void JumpHeightTestVisualize::setTest(const QString &testUid)
{

}
