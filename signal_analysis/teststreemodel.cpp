#include "teststreemodel.h"

#include <QDebug>

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "channelsutils.h"

namespace  {

QList<MetodicDefines::MetodicInfo> m_metList;

}

TestsTreeModel::TestsTreeModel(QObject *parent)
    : QStandardItemModel(parent)
{
    setHorizontalHeaderLabels(QStringList() << tr("Элементы") << "");
}

void TestsTreeModel::load()
{

}

void TestsTreeModel::openTest(const QString &testUid)
{
    if (!isTestOpened(testUid))
        doOpenTest(testUid);
}

void TestsTreeModel::closeTest(const QString &testUid)
{
    Q_UNUSED(testUid);
}

QList<QString> TestsTreeModel::getTests()
{
    QList<QString> retval;
    for (int i = 0; i < rowCount(); ++i)
        retval << index(i, 0).data(UidRole).toString();
    return retval;
}

bool TestsTreeModel::isTestOpened(const QString &testUid)
{
    for (int i = 0; i < rowCount(); ++i)
        if (index(i, 0).data(UidRole).toString() == testUid)
            return true;
    return false;
}

void TestsTreeModel::doOpenTest(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        DataDefines::PatientKard kard;
        DataProvider::getPatient(ti.patientUid, kard);

        auto *itemTest = new QStandardItem(kard.fio + " " +
                                           getMethodName(ti.metodUid) +
                                           " (" + ti.dateTime.toString("dd.MM.yyyy hh:mm") + ")");
        itemTest->setEditable(false);
        itemTest->setData(ti.uid, UidRole);
        itemTest->setIcon(QIcon(":/images/tree/test.png"));
        auto *itemTestClose = new QStandardItem("");
        itemTestClose->setEditable(false);
        itemTestClose->setData(ti.uid, UidRole);
        appendRow(QList<QStandardItem*>() << itemTest << itemTestClose);

        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto *itemProbe = new QStandardItem(pi.name);
                itemProbe->setEditable(false);
                itemProbe->setData(pi.uid, UidRole);
                itemProbe->setIcon(QIcon(":/images/tree/probe.png"));
                itemTest->appendRow(itemProbe);

                foreach (auto chi, pi.channels)
                {
                    QString chanName = ChannelsUtils::instance().channelName(chi.channelId);
                    auto *itemChan = new QStandardItem(chanName);
                    itemChan->setEditable(false);
                    itemChan->setData(chi.channelId, UidRole);
                    itemChan->setData(chi.uid, ChannelUidRole);
                    itemChan->setIcon(QIcon(":/images/tree/signal.png"));
                    itemProbe->appendRow(itemChan);
                }
            }
        }
    }
}

QString TestsTreeModel::getMethodName(const QString &metUid)
{
    if (m_metList.size() == 0)
        m_metList = DataProvider::getListMetodics();
    foreach (auto mi, m_metList)
        if (mi.uid == metUid)
            return mi.name;
    return "";
}

