#include "methodswidget.h"
#include "ui_methodswidget.h"

#include "metodicdefines.h"
#include "metodicsfactory.h"

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
