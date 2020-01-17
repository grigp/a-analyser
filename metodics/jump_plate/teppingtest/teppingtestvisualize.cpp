#include "teppingtestvisualize.h"
#include "ui_teppingtestvisualize.h"

TeppingTestVisualize::TeppingTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeppingTestVisualize)
{
    ui->setupUi(this);
}

TeppingTestVisualize::~TeppingTestVisualize()
{
    delete ui;
}

void TeppingTestVisualize::setTest(const QString &testUid)
{

}
