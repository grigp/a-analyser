#include "patientsmodel.h"

#include "dataprovider.h"
#include "datadefines.h"
#include "databasewigetdefines.h"

#include <QDebug>

PatientsModel::PatientsModel(QObject *parent)
    : QStandardItemModel(parent)
{

}

void PatientsModel::load()
{
    clear();

    auto uidList = DataProvider::getPatients();
    foreach (auto uid, uidList)
    {
        DataDefines::PatientKard patient;
        DataProvider::getPatient(uid, patient);

        addPatientInModel(patient);
    }
    setHorizontalHeaderLabels(QStringList() << tr("ФИО") << tr("Дата рождения")); // << tr("Пол"));

}

QString PatientsModel::addPatient(DataDefines::PatientKard &patient)
{
    QString uid = DataProvider::updatePatient(patient);
    load();
    return uid;
}

void PatientsModel::removePatient(const QString &uid)
{
    DataProvider::removePatient(uid);
    load();
}

void PatientsModel::addPatientInModel(DataDefines::PatientKard &patient)
{
    QStandardItem *itemFIO = new QStandardItem(patient.fio);
    itemFIO->setData(patient.uid, DatabaseWidgetDefines::PatientsModel::PatientUidRole);
    itemFIO->setData(patient.pp_uid, DatabaseWidgetDefines::PatientsModel::PatientPPUidRole);
    itemFIO->setEditable(false);
    QStandardItem *itemBorn = new QStandardItem(patient.born.toString("dd.MM.yyyy"));
    itemBorn->setEditable(false);
//    QStandardItem *itemSex = new QStandardItem(DataDefines::SexToText.value(
//                                                   static_cast<DataDefines::Sex>(patient.sex)));
//    itemSex->setData(patient.sex, DatabaseWidgetDefines::PatientsModel::PatientSexRole);
//    itemSex->setEditable(false);

    QList<QStandardItem*> retval;
    retval << itemFIO << itemBorn; // << itemSex;
    appendRow(retval);
}
