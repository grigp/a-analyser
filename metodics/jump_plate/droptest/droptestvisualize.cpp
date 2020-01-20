#include "droptestvisualize.h"
#include "ui_droptestvisualize.h"

DropTestVisualize::DropTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DropTestVisualize)
{
    ui->setupUi(this);
}

DropTestVisualize::~DropTestVisualize()
{
    delete ui;
}

void DropTestVisualize::setTest(const QString &testUid)
{

}
