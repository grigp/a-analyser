#include "summary.h"

#include "aanalyserapplication.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "factorsdefines.h"
#include "channelsutils.h"
#include "multifactordescriptor.h"
#include "multifactor.h"
#include "metodicsfactory.h"

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

        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(ti.metodUid);

        QList<QStandardItem*> lineFactors;
        QList<QStandardItem*> lineHdrProbes;
        QList<QStandardItem*> lineHdrChannels;
        QList<QStandardItem*> lineHdrMultiFactors;
        QList<QStandardItem*> lineHdrFactors;

        lineFactors << new QStandardItem(kard.fio + " \n" + mi.name + " \n" + ti.dateTime.toString("dd.MM.yyyy hh:mm"));
        lineHdrProbes << new QStandardItem();
        lineHdrChannels << new QStandardItem();
        lineHdrMultiFactors << new QStandardItem();
        lineHdrFactors << new QStandardItem();

        int fCount = 0;
        auto fgTest = calculateFactors(fCount, testUid);
        addCalculatedFactors(fgTest, BaseDefines::tlTest, lineFactors, lineHdrProbes, lineHdrChannels, lineHdrMultiFactors, lineHdrFactors);
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
                auto itemProbe = new QStandardItem(pi.name);
                lineHdrProbes << itemProbe;

                auto fgProbe = calculateFactors(fCount, testUid, pi.uid);
                addCalculatedFactors(fgProbe, BaseDefines::tlProbe, lineFactors, lineHdrProbes, lineHdrChannels, lineHdrMultiFactors, lineHdrFactors);
//                auto *itemProbe = new QStandardItem(pi.name);
//                itemProbe->setEditable(false);
//                itemProbe->setData(pi.uid, UidRole);
//                itemProbe->setIcon(QIcon(":/images/tree/probe.png"));
//                itemTest->appendRow(itemProbe);

                int ci = 0;
                foreach (auto chi, pi.channels)
                {
                    auto fgChannel = calculateFactors(fCount, testUid, pi.uid, chi.channelId);
                    if (fgChannel.size() > 0)
                    {
                        m_spanCells << SpanCellsInfo(1, lineHdrChannels.size(), fCount);

                        auto itemChan = new QStandardItem();
                        if (ci == 0)
                            itemChan->setText(ChannelsUtils::instance().channelName(chi.channelId));
                         lineHdrChannels << itemChan;
                        ++ci;

                        addCalculatedFactors(fgChannel, BaseDefines::tlChannel, lineFactors, lineHdrProbes, lineHdrChannels, lineHdrMultiFactors, lineHdrFactors);
                    }
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

        if (rowCount() == 0)
        {
            appendRow(lineHdrProbes);
            appendRow(lineHdrChannels);
            appendRow(lineHdrMultiFactors);
            appendRow(lineHdrFactors);
        }
        appendRow(lineFactors);
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

int Summary::spanCellsCount() const
{
    return m_spanCells.size();
}

Summary::SpanCellsInfo Summary::spanCells(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_spanCells.size());
    return m_spanCells.at(idx);
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

QList<MultiFactor *> Summary::calculateFactors(int& count, const QString &testUid, const QString &probeUid, const QString &channelId)
{
    QList<MultiFactor*> retval;
    retval.clear();
    count = 0;

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
        auto *fgd = app->getMultiFactor(level, i);
        if (fgd->isValid(testUid, probeUid, channelId))
        {
            auto fg = fgd->caclulate(testUid, probeUid, channelId);
            retval << fg;
            count += fg->size();
        }
    }

    return retval;
}

void Summary::addCalculatedFactors(QList<MultiFactor *> &fgs, BaseDefines::TestLevel level,
                                   QList<QStandardItem*> &line,
                                   QList<QStandardItem*> &lineHdrProbes,
                                   QList<QStandardItem*> &lineHdrChannels,
                                   QList<QStandardItem*> &lineHdrMultiFactors,
                                   QList<QStandardItem*> &lineHdrFactors)
{
    Q_UNUSED(level);
    foreach (auto fg, fgs)
    {
        for (int i = 0; i < fg->size(); ++i)
        {
            auto fv = fg->factorValueFormatted(fg->factorUid(i));
            auto *itemValue = new QStandardItem(fv);
            line << itemValue;

            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(fg->factorUid(i));
            auto itemFactor = new QStandardItem(fi.shortName());
            lineHdrFactors << itemFactor;

            auto itemMF = new QStandardItem();
            if (i == 0)
            {
                itemMF->setText(fg->name());
                m_spanCells << SpanCellsInfo(2, lineHdrMultiFactors.size(), fg->size());
            }
            lineHdrMultiFactors << itemMF;

            lineHdrChannels << new QStandardItem();
            lineHdrProbes << new QStandardItem();
        }

    }
}

