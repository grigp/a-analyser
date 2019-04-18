#include "patientswidget.h"
#include "ui_patientswidget.h"

#include "patientsmodel.h"

#include <QDebug>

PatientsWidget::PatientsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientsWidget)
  , m_mdlPatients(new PatientsModel(this))
{
    ui->setupUi(this);
    ui->tvPatients->setModel(m_mdlPatients);
}

PatientsWidget::~PatientsWidget()
{
    delete ui;
}

void PatientsWidget::onDbConnect()
{
    if (m_mdlPatients)
        m_mdlPatients->load();
}
