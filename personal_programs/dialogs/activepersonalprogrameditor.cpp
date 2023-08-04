#include "activepersonalprogrameditor.h"
#include "ui_activepersonalprogrameditor.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "personalprogramdefines.h"

ActivePersonalProgramEditor::ActivePersonalProgramEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActivePersonalProgramEditor)
{
    ui->setupUi(this);
    prepareParams();
}

ActivePersonalProgramEditor::~ActivePersonalProgramEditor()
{
    delete ui;
}

void ActivePersonalProgramEditor::setPersonalProgram(const QJsonObject &objPPAll)
{
    auto uidPatient = objPPAll["patient_uid"].toString();
    DataDefines::PatientKard pi;
    if (DataProvider::getPatient(uidPatient, pi))
    {
        ui->edPatient->setText(pi.fio);
        auto objPP = objPPAll["pp"].toObject();
        ui->edName->setText(objPP["name"].toString());

    }
}

QJsonObject ActivePersonalProgramEditor::personalProgram() const
{

}

void ActivePersonalProgramEditor::prepareParams()
{
    ui->cbMinTimeBetweenDP->clear();
    foreach (auto v, PersonalProgramDefines::MinTimeBetweenDPList)
        ui->cbMinTimeBetweenDP->addItem(PersonalProgramDefines::MinTimeBetweenDPNames.value(v), v);
    ui->cbMinTimeBetweenDP->setCurrentIndex(0);

    ui->cbMaxTimeBetweenDP->clear();
    foreach (auto v, PersonalProgramDefines::MaxTimeBetweenDPList)
        ui->cbMaxTimeBetweenDP->addItem(PersonalProgramDefines::MaxTimeBetweenDPNames.value(v), v);
    ui->cbMaxTimeBetweenDP->setCurrentIndex(7);
}
