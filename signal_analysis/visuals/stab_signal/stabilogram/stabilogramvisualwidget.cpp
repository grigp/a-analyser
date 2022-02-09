#include "stabilogramvisualwidget.h"
#include "ui_stabilogramvisualwidget.h"

#include "visuals.h"

StabilogramVisualWidget::StabilogramVisualWidget(BaseVisual* visual, QWidget *parent) :
    Visual(visual, parent),
    ui(new Ui::StabilogramVisualWidget)
{
    ui->setupUi(this);
}

StabilogramVisualWidget::~StabilogramVisualWidget()
{
    delete ui;
}

void StabilogramVisualWidget::calculate()
{

}
