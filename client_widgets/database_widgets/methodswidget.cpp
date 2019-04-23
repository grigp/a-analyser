#include "methodswidget.h"
#include "ui_methodswidget.h"

#include "metodicdefines.h"
#include "metodicsmodel.h"

MethodsWidget::MethodsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MethodsWidget)
  , m_model(new MetodicsModel(this))
{
    ui->setupUi(this);
}

MethodsWidget::~MethodsWidget()
{
    delete ui;
}

void MethodsWidget::onDbConnect()
{
    if (m_model)
    {
        m_model->load();
        ui->tvMetods->setModel(m_model);
    }
}

void MethodsWidget::editMetodParams()
{
    QModelIndexList selIdxs = ui->tvMetods->selectionModel()->selectedIndexes();
    if (m_model && selIdxs.size() > 0)
        m_model->editMetodicParams(this, selIdxs.at(0).row());
}
