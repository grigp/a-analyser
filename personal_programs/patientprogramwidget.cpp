#include "patientprogramwidget.h"
#include "ui_patientprogramwidget.h"

#include <QDebug>

#include "personalprogram.h"
#include "personalprogramdefines.h"
#include "dataprovider.h"
#include "dailyprograminfodelegate.h"
#include "testinfodelegate.h"

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
    ui->tvProgram->setItemDelegateForColumn(0, new DailyProgramInfoDelegate(ui->tvProgram));

    for (int i = 1; i < pp->columnCount(); ++i)
        ui->tvProgram->setItemDelegateForColumn(i, new TestInfoDelegate(ui->tvProgram));
}

void PatientProgramWidget::on_selectItem(QModelIndex idx)
{
    if (idx.column() > 0)
    {
        auto uidMethod = idx.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();

        if (uidMethod != "")
            m_selectedItem = idx;
    }
}
