#include "dynamicautowidget.h"
#include "ui_dynamicautowidget.h"

DynamicAutoWidget::DynamicAutoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DynamicAutoWidget)
{
    ui->setupUi(this);
}

DynamicAutoWidget::~DynamicAutoWidget()
{
    delete ui;
}

void DynamicAutoWidget::onDbConnect()
{

}
