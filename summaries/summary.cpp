#include "summary.h"

#include "aanalyserapplication.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "multifactordescriptor.h"
#include "multifactor.h"

#include <QDebug>

namespace  {

QList<MetodicDefines::MetodicInfo> m_metList;

}


Summary::Summary(QObject *parent)
    : QStandardItemModel (parent)
{

}

void Summary::addTest(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        DataDefines::PatientKard kard;
        DataProvider::getPatient(ti.patientUid, kard);

        auto fgTest = calculateFactors(testUid);
        addCalculatedFactors(fgTest, BaseDefines::tlTest);
//        auto *itemTest = new QStandardItem(kard.fio + " " +
//                                           getMethodName(ti.metodUid) +
//                                           " (" + ti.dateTime.toString("dd.MM.yyyy hh:mm") + ")");
//        itemTest->setEditable(false);
//        itemTest->setData(ti.uid, UidRole);
//        itemTest->setIcon(QIcon(":/images/tree/test.png"));
//        auto *itemTestClose = new QStandardItem("");
//        itemTestClose->setEditable(false);
//        itemTestClose->setData(ti.uid, UidRole);
//        appendRow(QList<QStandardItem*>() << itemTest << itemTestClose);

        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto fgProbe = calculateFactors(testUid, pi.uid);
                addCalculatedFactors(fgProbe, BaseDefines::tlProbe);
//                auto *itemProbe = new QStandardItem(pi.name);
//                itemProbe->setEditable(false);
//                itemProbe->setData(pi.uid, UidRole);
//                itemProbe->setIcon(QIcon(":/images/tree/probe.png"));
//                itemTest->appendRow(itemProbe);

                foreach (auto chi, pi.channels)
                {
                    auto fgChannel = calculateFactors(testUid, pi.uid, chi.channelId);
                    addCalculatedFactors(fgChannel, BaseDefines::tlChannel);
//                    QString chanName = ChannelsUtils::instance().channelName(chi.channelId);
//                    auto *itemChan = new QStandardItem(chanName);
//                    itemChan->setEditable(false);
//                    itemChan->setData(chi.channelId, UidRole);
//                    itemChan->setData(chi.uid, ChannelUidRole);
//                    itemChan->setIcon(QIcon(":/images/tree/signal.png"));
//                    itemProbe->appendRow(itemChan);
                }
            }
        }
    }
}

void Summary::open(const QString &fileName)
{
    m_fileName = fileName;
}

void Summary::save() const
{
    if (m_fileName != "")
    {

    }
}

QString Summary::name() const
{
    return m_name;
}

void Summary::setName(const QString &name)
{
    m_name = name;
}

QString Summary::getMethodName(const QString &metUid)
{
    if (m_metList.size() == 0)
        m_metList = DataProvider::getListMetodics();
    foreach (auto mi, m_metList)
        if (mi.uid == metUid)
            return mi.name;
    return "";
}

QList<MultiFactor *> Summary::calculateFactors(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    QList<MultiFactor*> retval;
    retval.clear();

    BaseDefines::TestLevel level = BaseDefines::tlNone;
    if (probeUid == "" && channelId == "")
        level = BaseDefines::tlTest;
    else
    if (channelId == "")
        level = BaseDefines::tlProbe;
    else
        level = BaseDefines::tlChannel;

    auto *app = static_cast<AAnalyserApplication*>(QApplication::instance());
    for (int i = 0; i < app->multiFactorCount(level); ++i)
    {
        auto *fg = app->getMultiFactor(level, i);
        if (fg->isValid(testUid, probeUid, channelId))
            retval << fg->caclulate(testUid, probeUid, channelId);
    }

    return retval;
}

void Summary::addCalculatedFactors(QList<MultiFactor *> fgs, BaseDefines::TestLevel level)
{
    qDebug() << "---------" << level;
    foreach (auto fg, fgs)
    {
        qDebug() << "----" << fg->uid() << fg->name();
        for (int i = 0; i < fg->size(); ++i)
        {
            qDebug() << fg->factorUid(i) << fg->factorValueFormatted(fg->factorUid(i));
            auto fv = fg->factorValueFormatted(fg->factorUid(i));

        }

    }
}

