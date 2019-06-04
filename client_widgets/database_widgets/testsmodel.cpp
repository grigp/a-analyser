#include "testsmodel.h"

#include "dataprovider.h"

TestsModel::TestsModel(QObject *parent)
    : QStandardItemModel(parent)
{

}

void TestsModel::load()
{
    clear();

    QStringList uidList = DataProvider::getTests();
    QList<MetodicDefines::MetodicInfo> metList = DataProvider::getListMetodisc();

    foreach (auto uid, uidList)
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTest(uid, ti))
        {
            DataDefines::PatientKard patient;
            DataProvider::getPatient(ti.patientUid, patient);
            QString metName = "";
            foreach (auto mi, metList)
                if (mi.uid == ti.metodUid)
                {
                    metName = mi.name;
                    break;
                }

            QStandardItem* itemPatient = new QStandardItem(patient.fio);
            itemPatient->setData(ti.patientUid, PatientUidRole);
            itemPatient->setEditable(false);

            QStandardItem* itemMetod = new QStandardItem(metName);
            itemMetod->setData(ti.metodUid, MetodicUidRole);
            itemMetod->setEditable(false);

            QStandardItem* itemDT = new QStandardItem(ti.dateTime.toString("dd.MM.yyyy hh:mm"));
            itemDT->setEditable(false);

            appendRow(QList<QStandardItem*>() << itemPatient << itemMetod << itemDT);
        }
    }
    setHorizontalHeaderLabels(QStringList() << "Пациент" << "Методика" << "Проведено");
}
