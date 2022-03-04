#include "bedsidescalestesterexecute.h"
#include "ui_bedsidescalestesterexecute.h"

BedsideScalesTesterExecute::BedsideScalesTesterExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BedsideScalesTesterExecute)
{
    ui->setupUi(this);
}

BedsideScalesTesterExecute::~BedsideScalesTesterExecute()
{
    delete ui;
}

void BedsideScalesTesterExecute::setParams(const QJsonObject &probeParams)
{

}
