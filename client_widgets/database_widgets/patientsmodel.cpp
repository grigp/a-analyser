#include "patientsmodel.h"

#include "dataprovider.h"
#include "datadefines.h"

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

        QStandardItem *itemFIO = new QStandardItem(patient.fio);
        itemFIO->setData(patient.uid, PatientUidRole);
        itemFIO->setEditable(false);
        QStandardItem *itemBorn = new QStandardItem(patient.born.toString("dd.MM.yyyy"));
        itemBorn->setEditable(false);
        QStandardItem *itemSex = new QStandardItem(DataDefines::SexToText.value(
                                                       static_cast<DataDefines::Sex>(patient.sex)));
        itemSex->setData(patient.sex, PatientSexRole);
        itemSex->setEditable(false);

        appendRow(QList<QStandardItem*>() << itemFIO << itemBorn << itemSex);
    }
    setHorizontalHeaderLabels(QStringList() << "ФИО" << "Дата рождения" << "Пол");

}

void PatientsModel::addPatient(DataDefines::PatientKard &patient)
{
    DataProvider::updatePatient(patient);
    load();
}

void PatientsModel::removePatient(const QString &uid)
{
    DataProvider::removePatient(uid);
    load();
}
