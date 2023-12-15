#include "mancoordppvisualwidget.h"
#include "ui_mancoordppvisualwidget.h"

ManCoordPPVisualWidget::ManCoordPPVisualWidget(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent) :
    PPVisual (visual, parent),
    ui(new Ui::ManCoordPPVisualWidget)
  , m_objPP(objPP)
{
    ui->setupUi(this);
}

ManCoordPPVisualWidget::~ManCoordPPVisualWidget()
{
    delete ui;
}

bool ManCoordPPVisualWidget::isValid()
{

}

void ManCoordPPVisualWidget::calculate()
{

}
