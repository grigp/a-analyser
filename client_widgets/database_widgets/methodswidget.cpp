#include "methodswidget.h"
#include "ui_methodswidget.h"

MethodsWidget::MethodsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MethodsWidget)
{
    ui->setupUi(this);
}

MethodsWidget::~MethodsWidget()
{
    delete ui;
}
