#include "stabtestvisualize.h"
#include "ui_stabtestvisualize.h"

#include "datadefines.h"
#include "dataprovider.h"

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

void StabTestVisualize::setTest(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTest(testUid, ti))
    {
        ui->lblTestInfo->setText(QString::number(ti.probes.size()));
    }
}
