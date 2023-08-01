#include "patientprogramwidget.h"
#include "ui_patientprogramwidget.h"

#include "personalprogram.h"
#include "dataprovider.h"

PatientProgramWidget::PatientProgramWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientProgramWidget)
{
    ui->setupUi(this);
}

PatientProgramWidget::~PatientProgramWidget()
{
    delete ui;
}

void PatientProgramWidget::assignPersonalProgram(const QString &patientUid)
{
    auto objPP = DataProvider::getActivePersonalProgramForPatient(patientUid);
    auto pp = new PersonalProgram(this);
    pp->load(objPP);
    ui->tvProgram->setModel(pp);
}
