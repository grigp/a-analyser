#include "testsmodel.h"

#include "dataprovider.h"
#include "aanalyserapplication.h"
#include "databasewigetdefines.h"

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

        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::removeTest,
                this, &TestsModel::onRemoveTest);
    });
}

void TestsModel::load()
{
    clear();

    m_metList = DataProvider::getListMetodics();
    QStringList uidList = DataProvider::getTests();

    foreach (auto uid, uidList)
        addTest(uid);
    setHorizontalHeaderLabels(QStringList() << tr("Пациент") << tr("Методика") << tr("Проведено"));
}

void TestsModel::onNewTest(const QString &testUid)
{
    addTest(testUid);
}

void TestsModel::onRemoveTest(const QString &testUid)
{
    int line = -1;
    for (int i = 0; i < rowCount(); ++i)
    {
        auto uid = index(i, DatabaseWidgetDefines::TestsModel::ColPatient).data(DatabaseWidgetDefines::TestsModel::TestUidRole).toString();
        if (uid == testUid)
        {
            line = i;
            break;
        }
    }
    if (line > -1)
    {
        removeRows(line, 1);
    }
}

void TestsModel::addTest(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        DataDefines::PatientKard patient;
        DataProvider::getPatient(ti.patientUid, patient);
        QString metName = "";
        QString metImageName = "";
        foreach (auto mi, m_metList)
        {
            if (mi.uid == ti.metodUid)
            {
                metName = mi.name;
                metImageName = mi.imageName;
                break;
            }
        }

//        QStandardItem* itemPatient = new QStandardItem(patient.fio);
        QStandardItem* itemPatient = new QStandardItem(patient.fio + "\n" + metName); // + "\n" + ti.dateTime.toString("dd.MM.yyyy hh:mm"));
        itemPatient->setData(testUid, DatabaseWidgetDefines::TestsModel::TestUidRole);
        itemPatient->setData(ti.patientUid, DatabaseWidgetDefines::TestsModel::PatientUidRole);
        itemPatient->setEditable(false);

        QStandardItem* itemMetod = new QStandardItem(metName);
        itemMetod->setIcon(QIcon(":/images/Methodics/" + metImageName));
        itemMetod->setData(ti.metodUid, DatabaseWidgetDefines::TestsModel::MetodicUidRole);
        itemMetod->setEditable(false);

        QStandardItem* itemDT = new QStandardItem(ti.dateTime.toString("dd.MM.yyyy hh:mm"));
        itemDT->setEditable(false);

        appendRow(QList<QStandardItem*>() << itemPatient << itemMetod << itemDT);
    }
}
