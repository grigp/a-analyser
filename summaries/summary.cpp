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
        //! Получить карточку пациента
        DataDefines::PatientKard kard;
        DataProvider::getPatient(ti.patientUid, kard);

        //! Получить данные о методике
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(ti.metodUid);

        //! Строци итемов, показателей и заголовка, которые могут быть добавлены
        QList<QStandardItem*> lineFactors;          //! Значения показателей для теста
        QList<QStandardItem*> lineHdrProbes;        //! Строка заголовка - названия проб
        QList<QStandardItem*> lineHdrChannels;      //! Строка заголовка - названия каналов
        QList<QStandardItem*> lineHdrMultiFactors;  //! Строка заголовка - названия групп показателей
        QList<QStandardItem*> lineHdrFactors;       //! Строка заголовка - названия показателей

        //! Добавление первого столбца - пациент + методика + дата и время проведения
        lineFactors << new QStandardItem(kard.fio + " \n" + mi.name + " \n" + ti.dateTime.toString("dd.MM.yyyy hh:mm"));
        //! Добавление пустых итемов в заголовок
        lineHdrProbes << new QStandardItem();
        lineHdrChannels << new QStandardItem();
        lineHdrMultiFactors << new QStandardItem();
        lineHdrFactors << new QStandardItem();

        int fCount = 0;

        //! Расчет показателей уровня теста
        auto fgTest = calculateFactors(fCount, testUid);
        m_spanCells << SpanCellsInfo(0, lineHdrProbes.size(), fCount);   //! Соединяем на линии проб
        m_spanCells << SpanCellsInfo(1, lineHdrChannels.size(), fCount); //! Соединяем на линии каналов
        //! Добавляем итемы с показателями уровня теста
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

        //! Цикл по пробам
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            //! Данные пробы
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                //! Итем названия пробы
                auto itemProbe = new QStandardItem(pi.name);
                lineHdrProbes << itemProbe;
                int spp = lineHdrProbes.size() - 1;  //! StartPosProbe

                //! Показатели уровня пробы
                auto fgProbe = calculateFactors(fCount, testUid, pi.uid);
                m_spanCells << SpanCellsInfo(1, lineHdrChannels.size(), fCount);
                //! Добавляем итемы с показателями уровня пробы
                addCalculatedFactors(fgProbe, BaseDefines::tlProbe, lineFactors, lineHdrProbes, lineHdrChannels, lineHdrMultiFactors, lineHdrFactors);
//                auto *itemProbe = new QStandardItem(pi.name);
//                itemProbe->setEditable(false);
//                itemProbe->setData(pi.uid, UidRole);
//                itemProbe->setIcon(QIcon(":/images/tree/probe.png"));
//                itemTest->appendRow(itemProbe);

                //! Цикл по каналам
                int ci = 0;
                foreach (auto chi, pi.channels)
                {
                    //! Расчет показателей уровня канала
                    auto fgChannel = calculateFactors(fCount, testUid, pi.uid, chi.channelId);
                    if (fgChannel.size() > 0)
                    {
                        m_spanCells << SpanCellsInfo(1, lineHdrChannels.size(), fCount);

                        //! Итем названия канала
                        auto itemChan = new QStandardItem();
                        if (ci == 0)
                            itemChan->setText(ChannelsUtils::instance().channelName(chi.channelId));
                         lineHdrChannels << itemChan;
                        ++ci;

                        //! Добавляем итемы с показателями уровня канала
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

                m_spanCells << SpanCellsInfo(0, spp, lineHdrProbes.size() - spp);
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
    bool first = true;
    foreach (auto fg, fgs)
    {
        for (int i = 0; i < fg->size(); ++i)
        {
            //! Значение
            auto fv = fg->factorValueFormatted(fg->factorUid(i));
            auto *itemValue = new QStandardItem(fv);
            line << itemValue;

            //! Название показателя
            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(fg->factorUid(i));
            auto itemFactor = new QStandardItem(fi.shortName());
            lineHdrFactors << itemFactor;

            //! Группа показателей
            auto itemMF = new QStandardItem();
            if (i == 0)
            {
                itemMF->setText(fg->name());
                m_spanCells << SpanCellsInfo(2, lineHdrMultiFactors.size(), fg->size());
                m_spanCells << SpanCellsInfo(2, lineHdrMultiFactors.size(), fg->size());
            }
            lineHdrMultiFactors << itemMF;

            if (level != BaseDefines::tlChannel ||
                (level == BaseDefines::tlChannel && !first))
            {
                //! Канал
                lineHdrChannels << new QStandardItem();
                //! Проба
                lineHdrProbes << new QStandardItem();
            }
            first = false;
        }

    }
}

