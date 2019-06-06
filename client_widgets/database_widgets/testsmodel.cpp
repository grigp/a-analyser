#include "testsmodel.h"

#include "dataprovider.h"
#include "aanalyserapplication.h"

#include <QApplication>
#include <QTimer>
#include <QDebug>

namespace  {

QList<MetodicDefines::MetodicInfo> m_metList;

}

TestsModel::TestsModel(QObject *parent)
    : QStandardItemModel(parent)
{
    QTimer::singleShot(100, [=]()
    {
        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::newTest,
                this, &TestsModel::onNewTest);
    });
}

void TestsModel::load()
{
    clear();

    m_metList = DataProvider::getListMetodisc();
    QStringList uidList = DataProvider::getTests();

    foreach (auto uid, uidList)
        addTest(uid);
    setHorizontalHeaderLabels(QStringList() << "Пациент" << "Методика" << "Проведено");
}

void TestsModel::onNewTest(const QString &testUid)
{
    addTest(testUid);
}

void TestsModel::addTest(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTest(testUid, ti))
    {
        DataDefines::PatientKard patient;
        DataProvider::getPatient(ti.patientUid, patient);
        QString metName = "";
        foreach (auto mi, m_metList)
        {
            if (mi.uid == ti.metodUid)
            {
                metName = mi.name;
                break;
            }
        }

        QStandardItem* itemPatient = new QStandardItem(patient.fio);
        itemPatient->setData(testUid, TestUidRole);
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
