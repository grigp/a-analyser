#include "trentakeputvisualize.h"
#include "ui_trentakeputvisualize.h"

#include "trentakeputtestcalculator.h"

TrenTakePutVisualize::TrenTakePutVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenTakePutVisualize)
{
    ui->setupUi(this);
}

TrenTakePutVisualize::~TrenTakePutVisualize()
{
    delete ui;
}

void TrenTakePutVisualize::setTest(const QString &testUid)
{

}
