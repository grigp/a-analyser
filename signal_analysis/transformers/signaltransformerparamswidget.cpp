#include "signaltransformerparamswidget.h"
#include "ui_signaltransformerparamswidget.h"

SignalTransformerParamsWidget::SignalTransformerParamsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalTransformerParamsWidget)
{
    ui->setupUi(this);
}

SignalTransformerParamsWidget::~SignalTransformerParamsWidget()
{
    delete ui;
}
