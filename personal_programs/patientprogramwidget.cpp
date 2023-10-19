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

void PatientProgramWidget::assignPersonalProgram(const QString &uidPP)
{
    auto objPP = DataProvider::getPersonalProgramByUid(uidPP);
    auto pp = new PersonalProgram(this);
    pp->load(objPP);
    ui->tvProgram->setModel(pp);
    ui->tvProgram->setItemDelegateForColumn(0, new DailyProgramInfoDelegate(ui->tvProgram));

    for (int i = 1; i < pp->columnCount(); ++i)
        ui->tvProgram->setItemDelegateForColumn(i, new TestInfoDelegate(ui->tvProgram));
}

QString PatientProgramWidget::namePP() const
{
    return static_cast<PersonalProgram*>(ui->tvProgram->model())->name();
}

void PatientProgramWidget::on_selectItem(QModelIndex idx)
{
    m_selectedIndex = QModelIndex();
    if (idx.column() > 0)
    {
        auto uidMethod = idx.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();

        if (uidMethod != "")
        {
            m_selectedIndex = idx;
        }
    }
}
