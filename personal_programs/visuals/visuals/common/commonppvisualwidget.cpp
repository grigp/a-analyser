#include "commonppvisualwidget.h"
#include "ui_commonppvisualwidget.h"

CommonPPVisualWidget::CommonPPVisualWidget(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent) :
    PPVisual(visual, objPP, parent),
    ui(new Ui::CommonPPVisualWidget)
{
    ui->setupUi(this);
}

CommonPPVisualWidget::~CommonPPVisualWidget()
{
    delete ui;
}

bool CommonPPVisualWidget::isValid()
{
    return true;   //! Подходит для любой индивидуальной программы
}

void CommonPPVisualWidget::calculate()
{

}

void CommonPPVisualWidget::print()
{

}
