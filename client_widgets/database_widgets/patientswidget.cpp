#include "patientswidget.h"
#include "ui_patientswidget.h"

PatientsWidget::PatientsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientsWidget)
{
    ui->setupUi(this);
}

PatientsWidget::~PatientsWidget()
{
    delete ui;
}
