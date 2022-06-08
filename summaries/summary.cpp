#include "summary.h"

#include "aanalyserapplication.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "factorsdefines.h"
#include "channelsutils.h"
#include "multifactordescriptor.h"
#include "multifactor.h"
#include "metodicsfactory.h"

#include <QUuid>
#include <QJsonDocument>
#include <QJsonArray>
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
    if (m_kind == SummaryDefines::skAll)
        addTestAll(testUid);
    else
    if (m_kind == SummaryDefines::skPrimary)
        addTestPrimary(testUid);
}

void Summary::open(const QString &fileName)
{
    m_fileName = fileName;
}

void Summary::save() const
{
    if (m_kind == SummaryDefines::skAll)
        saveAll();
    else
    if (m_kind == SummaryDefines::skPrimary)
        savePrimary();
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

void Summary::addTestAll(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        //! Получить карточку пациента
        DataDefines::PatientKard kard;
        DataProvider::getPatient(ti.patientUid, kard);
//        DataProvider::getPrimaryFactors(testUid);

        //! Получить данные о методике
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(ti.metodUid);

        //! Строки итемов, показателей и заголовка, которые могут быть добавлены
        QList<QStandardItem*> lineFactors;          //! Значения показателей для теста
        lineFactors.clear();
        QList<QStandardItem*> lineHdrProbes;        //! Строка заголовка - названия проб
        lineHdrProbes.clear();
        QList<QStandardItem*> lineHdrChannels;      //! Строка заголовка - названия каналов
        lineHdrChannels.clear();
        QList<QStandardItem*> lineHdrMultiFactors;  //! Строка заголовка - названия групп показателей
        lineHdrMultiFactors.clear();
        QList<QStandardItem*> lineHdrFactors;       //! Строка заголовка - названия показателей
        lineHdrFactors.clear();

        //! Добавление первого столбца - пациент + методика + дата и время проведения
        auto itemTH = createItem(lineFactors, kard.fio + " \n" + ti.dateTime.toString("dd.MM.yyyy hh:mm"));
        itemTH->setData(kard.fio, PatientFioRole);
        itemTH->setData(ti.dateTime, TestDateTimeRole);

        //! Добавление пустых итемов в заголовок
        m_uidMethodic = mi.uid;
        addRootItem(lineHdrProbes, mi);
        lineHdrChannels << new QStandardItem();
        lineHdrMultiFactors << new QStandardItem();
        lineHdrFactors << new QStandardItem();

        int fCount = 0;

        //! Расчет показателей уровня теста
        auto fgTest = calculateFactors(fCount, testUid);
        m_spanCells << SpanCellsInfo(RowProbes, lineHdrProbes.size(), fCount);   //! Соединяем на линии проб
        m_spanCells << SpanCellsInfo(RowChannels, lineHdrChannels.size(), fCount); //! Соединяем на линии каналов
        //! Добавляем итемы с показателями уровня теста
        addCalculatedFactors(fgTest, BaseDefines::tlTest, lineFactors, lineHdrProbes, lineHdrChannels, lineHdrMultiFactors, lineHdrFactors);

        //! Цикл по пробам
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            //! Данные пробы
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                //! Итем названия пробы
                auto itemProbe = createItem(lineHdrProbes, pi.name);
                itemProbe->setData(i, ProbeNumberRole);
                itemProbe->setData(pi.name, ProbeNameRole);
                int spp = lineHdrProbes.size() - 1;  //! StartPosProbe

                //! Показатели уровня пробы
                auto fgProbe = calculateFactors(fCount, testUid, pi.uid);
                m_spanCells << SpanCellsInfo(RowChannels, lineHdrChannels.size(), fCount);
                //! Добавляем итемы с показателями уровня пробы
                addCalculatedFactors(fgProbe, BaseDefines::tlProbe, lineFactors, lineHdrProbes, lineHdrChannels, lineHdrMultiFactors, lineHdrFactors);

                //! Цикл по каналам
                int ci = 0;
                foreach (auto chi, pi.channels)
                {
                    //! Расчет показателей уровня канала
                    auto fgChannel = calculateFactors(fCount, testUid, pi.uid, chi.channelId);
                    if (fgChannel.size() > 0)
                    {
                        m_spanCells << SpanCellsInfo(RowChannels, lineHdrChannels.size(), fCount);

                        //! Итем названия канала
                        QString cn = "";
                        if (ci == 0)
                            cn = ChannelsUtils::instance().channelName(chi.channelId);
                        auto itemChan = createItem(lineHdrChannels, cn);
                        itemChan->setData(chi.channelId, ChannelIdRole);
                        itemChan->setData(cn, ChannelNameRole);
                        ++ci;

                        //! Добавляем итемы с показателями уровня канала
                        addCalculatedFactors(fgChannel, BaseDefines::tlChannel, lineFactors, lineHdrProbes, lineHdrChannels, lineHdrMultiFactors, lineHdrFactors);
                    }
                }

                m_spanCells << SpanCellsInfo(RowProbes, spp, lineHdrProbes.size() - spp);
            }
        }

        if (rowCount() == 0)
        {
            appendRow(lineHdrProbes);
            appendRow(lineHdrChannels);
            appendRow(lineHdrMultiFactors);
            appendRow(lineHdrFactors);
            appendRow(lineFactors);
        }
        else
            appendRow(sortItems(lineFactors));
    }
}

void Summary::addTestPrimary(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        //! Получить карточку пациента
        DataDefines::PatientKard kard;
        DataProvider::getPatient(ti.patientUid, kard);

        //! Получить данные о методике
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(ti.metodUid);

        //! Получить первичные показатели
        auto factors = DataProvider::getPrimaryFactors(testUid);

        //! Строки итемов, показателей и заголовка, которые могут быть добавлены
        QList<QStandardItem*> lineFactors;          //! Значения показателей для теста
        lineFactors.clear();
        QList<QStandardItem*> lineHeader;           //! Строка заголовка
        lineHeader.clear();

        //! Добавление первого столбца - пациент + методика + дата и время проведения
        auto itemTH = createItem(lineFactors, kard.fio + " \n" + ti.dateTime.toString("dd.MM.yyyy hh:mm"));
        itemTH->setData(kard.fio, PatientFioRole);
        itemTH->setData(ti.dateTime, TestDateTimeRole);

        //! Добавление пустых итемов в заголовок
        m_uidMethodic = mi.uid;
        addRootItem(lineHeader, mi);

        foreach (auto factor, factors)
        {
            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factor.uid());

            //! Значение
            auto *itemValue = new QStandardItem(QString::number(factor.value(), 'f', fi.format()));
            itemValue->setData(factor.value(), FactorValueRole);
            itemValue->setData(factor.uid(), FactorUidRole);
            lineFactors << itemValue;

            //! Название показателя
            auto itemFactor = new QStandardItem(fi.shortName());
            itemFactor->setData(factor.uid(), FactorUidRole);
            itemFactor->setData(fi.name(), FactorNameRole);
            itemFactor->setData(fi.shortName(), FactorShortNameRole);
            itemFactor->setData(fi.measure(), FactorMeasureRole);
            itemFactor->setData(fi.format(), FactorFormatRole);
            lineHeader << itemFactor;
        }

        if (rowCount() == 0)
        {
            appendRow(lineHeader);
            appendRow(lineFactors);
        }
        else
            appendRow(sortItems(lineFactors));
    }
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
            itemValue->setData(fg->factorValue(i), FactorValueRole);
            itemValue->setData(fg->factorUid(i), FactorUidRole);
            line << itemValue;

            //! Название показателя
            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(fg->factorUid(i));
            auto itemFactor = new QStandardItem(fi.shortName());
            itemFactor->setData(fg->factorUid(i), FactorUidRole);
            itemFactor->setData(fi.name(), FactorNameRole);
            itemFactor->setData(fi.shortName(), FactorShortNameRole);
            itemFactor->setData(fi.measure(), FactorMeasureRole);
            itemFactor->setData(fi.format(), FactorFormatRole);
            lineHdrFactors << itemFactor;

            //! Группа показателей
            auto itemMF = new QStandardItem();
            if (i == 0)
            {
                itemMF->setText(fg->name());
                itemMF->setData(fg->uid(), MultiFactorUidRole);
                itemMF->setData(fg->name(), MultiFactorNameRole);
                m_spanCells << SpanCellsInfo(RowMultifactors, lineHdrMultiFactors.size(), fg->size());
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

QStandardItem *Summary::createItem(QList<QStandardItem *> &line, const QString text)
{
    auto item = new QStandardItem(text);
    item->setEditable(false);
    line << item;
    return item;
}

void Summary::addRootItem(QList<QStandardItem *> &line, MetodicDefines::MetodicInfo &mi)
{
    auto itemRoot = createItem(line, "");
    m_uid = QUuid::createUuid().toString();
    itemRoot->setData(m_uid, SummaryUidRole);
    itemRoot->setData(m_kind, SummaryKindRole);
    itemRoot->setData(mi.uid, MethodicUIdRole);
    itemRoot->setData(mi.name, MethodicNameRole);
    itemRoot->setData(version(), VersionRole);
}

QList<QStandardItem *> Summary::sortItems(QList<QStandardItem *> &line)
{
    //! Строка с показателями
    int rowFN = RowFactors;
    if (m_kind == SummaryDefines::skPrimary)
        rowFN = RowPrimaryFactors;

    //! Возвращаемый список
    QList<QStandardItem*> retval;
    retval.clear();

    //! Передаем данные по тесту
    retval << line.at(0);
    line.removeFirst();

    for (int i = 1; i < columnCount(); ++i)
    {
        auto factorUid = index(rowFN, i).data(FactorUidRole).toString();
        for (int j = 0; j < line.size(); ++j)
            if (line.at(j)->data(FactorUidRole).toString() == factorUid)
            {
                retval << line.at(j);
                line.removeAt(j);
                break;
            }
    }

    return retval;
}

void Summary::saveAll() const
{
    if (m_fileName != "")
    {
        QFile file(m_fileName);
        file.remove();
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            //! Заголовок
            QJsonObject root;
            root["uid"] = m_uid;
            root["name"] = m_name;
            if (m_kind == SummaryDefines::skAll)
                root["kind"] = "all";
            else
            if (m_kind == SummaryDefines::skPrimary)
                root["kind"] = "primary";
            root["methodic_uid"] = m_uidMethodic;
            auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(m_uidMethodic);
            root["methodic_name"] = mi.name;
            root["version"] = version();

            //! Таблица показателей с отдльно обрабатываемым заголовком
            QJsonArray rows;
            for (int i = 0; i < rowCount(); ++i)
            {
                //! По столбцам для строки
                QJsonArray columns;
                for (int j = 0; j < columnCount(); ++j)
                {
                    //! Всегда выводим текст итема
                    QJsonObject item;
                    item["text"] = index(i, j).data().toString();

                    //! В зависимости от номера строки
                    if (i == 0 && index(i, j).data(ProbeNameRole).toString() != "")
                    {
                        item["probe_number"] = index(i, j).data(ProbeNumberRole).toInt();
                        item["probe_name"] = index(i, j).data(ProbeNameRole).toString();
                    }
                    else
                    if (i == 1 && index(i, j).data(ChannelIdRole).toString() != "")
                    {
                        item["channel_id"] = index(i, j).data(ChannelIdRole).toString();
                        item["channel_name"] = index(i, j).data(ChannelNameRole).toString();
                    }
                    else
                    if (i == 2 && index(i, j).data(MultiFactorUidRole).toString() != "")
                    {
                        item["multifactor_uid"] = index(i, j).data(MultiFactorUidRole).toString();
                        item["multifactor_name"] = index(i, j).data(MultiFactorNameRole).toString();
                    }
                    else
                    if (i == 3 && index(i, j).data(FactorUidRole).toString() != "")
                    {
                        item["uid"] = index(i, j).data(FactorUidRole).toString();
                        item["name"] = index(i, j).data(FactorNameRole).toString();
                        item["short_name"] = index(i, j).data(FactorShortNameRole).toString();
                        item["measure"] = index(i, j).data(FactorMeasureRole).toString();
                        item["format"] = index(i, j).data(FactorFormatRole).toInt();
                    }
                    if (i >= 4)   //! Строки значений показателей для тестов
                    {
                        if (j == 0)  //! Заголовок. ФИО и дата+время проведения
                        {
                            item["patient_fio"] = index(i, j).data(PatientFioRole).toString();
                            item["datetime"] = index(i, j).data(TestDateTimeRole).toDateTime().toString("dd.MM.yyyy hh:mm");
                        }
                        else
                        if (j > 0)   //! Показатели
                        {
                            item["uid"] = index(i, j).data(FactorUidRole).toString();
                            item["value"] = index(i, j).data(FactorValueRole).toDouble();
                        }
                    }

                    columns << item;
                }
                rows << columns;
            }

            root["table"] = rows;

            //! "Объединенные итемы"
            QJsonArray spans;
            foreach (auto span, m_spanCells)
            {
                QJsonObject so;
                so["row"] = span.row;
                so["col"] = span.col;
                so["width"] = span.width;
                spans << so;
            }
            root["span_cells"] = spans;

            //! Оформление документа
            QJsonDocument doc(root);
            QByteArray ba = doc.toJson();
            file.write(ba);

            file.close();
        }
    }
}

void Summary::savePrimary() const
{
    if (m_fileName != "")
    {
        QFile file(m_fileName);
        file.remove();
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            //! Заголовок
            QJsonObject root;
            root["uid"] = m_uid;
            root["name"] = m_name;
            if (m_kind == SummaryDefines::skAll)
                root["kind"] = "all";
            else
            if (m_kind == SummaryDefines::skPrimary)
                root["kind"] = "primary";
            root["methodic_uid"] = m_uidMethodic;
            auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(m_uidMethodic);
            root["methodic_name"] = mi.name;
            root["version"] = version();

            //! Таблица показателей с отдльно обрабатываемым заголовком
            QJsonArray rows;
            for (int i = 0; i < rowCount(); ++i)
            {
                //! По столбцам для строки
                QJsonArray columns;
                for (int j = 0; j < columnCount(); ++j)
                {
                    //! Всегда выводим текст итема
                    QJsonObject item;
                    item["text"] = index(i, j).data().toString();

                    //! В зависимости от номера строки
                    if (i == 0 && index(i, j).data(FactorUidRole).toString() != "")
                    {
                        item["uid"] = index(i, j).data(FactorUidRole).toString();
                        item["name"] = index(i, j).data(FactorNameRole).toString();
                        item["short_name"] = index(i, j).data(FactorShortNameRole).toString();
                        item["measure"] = index(i, j).data(FactorMeasureRole).toString();
                        item["format"] = index(i, j).data(FactorFormatRole).toInt();
                    }
                    if (i >= 1)   //! Строки значений показателей для тестов
                    {
                        if (j == 0)  //! Заголовок. ФИО и дата+время проведения
                        {
                            item["patient_fio"] = index(i, j).data(PatientFioRole).toString();
                            item["datetime"] = index(i, j).data(TestDateTimeRole).toDateTime().toString("dd.MM.yyyy hh:mm");
                        }
                        else
                        if (j > 0)   //! Показатели
                        {
                            item["uid"] = index(i, j).data(FactorUidRole).toString();
                            item["value"] = index(i, j).data(FactorValueRole).toDouble();
                        }
                    }

                    columns << item;
                }
                rows << columns;
            }

            root["table"] = rows;

            //! "Объединенные итемы"
            QJsonArray spans;
            foreach (auto span, m_spanCells)
            {
                QJsonObject so;
                so["row"] = span.row;
                so["col"] = span.col;
                so["width"] = span.width;
                spans << so;
            }
            root["span_cells"] = spans;

            //! Оформление документа
            QJsonDocument doc(root);
            QByteArray ba = doc.toJson();
            file.write(ba);

            file.close();
        }

    }
}

