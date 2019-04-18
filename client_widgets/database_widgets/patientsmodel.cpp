#include "patientsmodel.h"

#include "dataprovider.h"
#include "datadefines.h"

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
        DataDefines::PatientKard kard;
        DataProvider::getPatient(uid, kard);

        QStandardItem *itemFIO = new QStandardItem(kard.fio);
        itemFIO->setEditable(false);
        QStandardItem *itemBorn = new QStandardItem(kard.born.toString());
        itemBorn->setEditable(false);
        QStandardItem *itemSex = new QStandardItem(kard.sex);
        itemSex->setEditable(false);

        appendRow(QList<QStandardItem*>() << itemFIO << itemBorn << itemSex);
    }
    setHorizontalHeaderLabels(QStringList() << "ФИО" << "Дата рождения" << "Пол");

}
