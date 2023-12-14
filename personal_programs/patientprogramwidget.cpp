#include "patientprogramwidget.h"
#include "ui_patientprogramwidget.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "personalprogram.h"
#include "personalprogramdefines.h"
#include "dataprovider.h"
#include "dailyprograminfodelegate.h"
#include "testinfodelegate.h"
#include "metodicsfactory.h"

PatientProgramWidget::PatientProgramWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientProgramWidget)
{
    ui->setupUi(this);
    connect(ui->tvProgram, &QTableView::doubleClicked, this, &PatientProgramWidget::on_doubleClicked);
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
        //! Название методики в статусе
        auto uidMethod = idx.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();
        MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
        auto mi = metFactory->metodic(uidMethod);
        ui->lblMethodName->setText(tr("Методика") + " - " + mi.name);

        //! Дата и время проведения теста в статусе
        auto uidTest = idx.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(uidTest, ti))
            ui->lblDateTime->setText(tr("Проведен") + " - " + ti.dateTime.toString("dd:MM:yyyy hh:mm"));

        if (uidMethod != "")
        {
            m_selectedIndex = idx;
            emit selectItem(idx);
        }
    }
}

void PatientProgramWidget::on_doubleClicked(QModelIndex idx)
{
    emit doubleClicked(idx);
}
