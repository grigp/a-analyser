#include "octaedronvisualize.h"
#include "ui_octaedronvisualize.h"

OctaedronVisualize::OctaedronVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OctaedronVisualize)
{
    ui->setupUi(this);
}

OctaedronVisualize::~OctaedronVisualize()
{
    delete ui;
}

void OctaedronVisualize::setTest(const QString &testUid)
{

}
