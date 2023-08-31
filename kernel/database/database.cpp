#include "database.h"

#include "datadefines.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "settingsprovider.h"
#include "normsmanager.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <QTimer>
#include <QDebug>

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(100, [=]()
    {
        emit connected();
    });
}

DataBase::~DataBase()
{
    emit disconnected();
}

QStringList DataBase::getPatients() const
{
    QStringList retval;

    QDir dir = patientsDir();
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
        retval << fileInfo.fileName();
    return retval;
}

bool DataBase::getPatient(const QString &uid, DataDefines::PatientKard &patient) const
{
    QDir dir = patientsDir();
    QJsonObject patObj;
    if (readTableRec(dir.absoluteFilePath(uid), patObj))
    {
        patient.uid = uid;
        patient.fio = patObj["fio"].toString();
        patient.born = QDate::fromString(patObj["born"].toString(), "dd.MM.yyyy");
        patient.sex = static_cast<DataDefines::Sex>(patObj["sex"].toInt());
        patient.massa = patObj["massa"].toInt();
        patient.height = patObj["height"].toInt();
        patient.pp_uid = patObj["personal_program"].toString();
        return true;
    }
    return false;
}

QString DataBase::updatePatient(const DataDefines::PatientKard &patient)
{
    if (patient.uid == "" || !patientExists(patient.uid))
        return createPatientRec(patient);
    else
    {
        updatePatientRec(patient);
        return patient.uid;
    }
}

QVariant DataBase::patientData(const QString &uid, const QString &name) const
{
    QDir dir = patientsDir();
    QJsonObject patObj;
    if (readTableRec(dir.absoluteFilePath(uid), patObj))
    {
        QJsonArray data = patObj["data"].toArray();
        for (int i = 0; i < data.size(); ++i)
        {
            auto obj = data.at(i).toObject();
            if (obj["name"].toString() == name)
            {
                auto value = obj["value"];
                if (value.isNull() || value.isArray() || value.isObject() || value.isUndefined())
                    return QVariant();
                else
                if (value.isBool())
                    return value.toBool();
                else
                if (value.isDouble())
                    return value.toDouble();
                else
                if (value.isString())
                    return value.toString();
            }
        }
    }
    return QVariant();
}

void DataBase::patientSetData(const QString &uid, const QString &name, const QVariant value)
{
    QDir dir = patientsDir();
    QJsonObject patObj;
    if (readTableRec(dir.absoluteFilePath(uid), patObj))
    {
        QJsonArray data = patObj["data"].toArray();
        bool fnd = false;
        for (int i = 0; i < data.size(); ++i)
        {
            auto obj = data.at(i).toObject();
            if (obj["name"].toString() == name)
            {
                obj["value"] = QJsonValue::fromVariant(value);
                data.replace(i, obj);
                fnd = true;
            }
        }

        if (!fnd)
        {
            QJsonObject obj;
            obj["name"] = name;
            obj["value"] = QJsonValue::fromVariant(value);
            data << obj;
        }

        patObj["data"] = data;
        writeTableRec(dir.absoluteFilePath(uid), patObj);
    }
}

void DataBase::removePatient(const QString &uid)
{
    if (patientExists(uid))
    {
        auto tests = getTests();
        foreach (auto testUid, tests)
        {
            DataDefines::TestInfo ti;
            if (getTestInfo(testUid, ti))
                if (ti.patientUid == uid)
                    static_cast<AAnalyserApplication*>(QApplication::instance())->deleteTest(testUid);
        }

        QDir dir = patientsDir();
        QFile fPatientRec(dir.absoluteFilePath(uid));
        fPatientRec.remove();
    }
}

QString DataBase::addTestStart(const QString &patientUid, const QString &metodUid)
{
    //! uid нового теста
    auto testUid = QUuid::createUuid().toString();

    //! Создание записи в таблице тестов
    QDir dirTest = testsDir();
    QJsonObject testObj;
    testObj["patientUid"] = patientUid;
    testObj["metodUid"] = metodUid;
    testObj["datetime"] = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");

    // Параметры методики на момент проведения
    auto metodic = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(metodUid);
    testObj["params"] = metodic.params;

    writeTableRec(dirTest.absoluteFilePath(testUid), testObj);

    return testUid;
}

void DataBase::addTestFinish(const QString &testUid)
{
    //! Известим мир
    emit newTest(testUid);
}

QString DataBase::addProbe(const QString &testUid, const int step, const QString &name)
{
    //! uid новой пробы
    auto probeUid = QUuid::createUuid().toString();

    //! Запись в данные теста
    QDir dir = testsDir();
    QJsonObject testObj;
    if (readTableRec(dir.absoluteFilePath(testUid), testObj))
    {
        QJsonObject probe;
        probe["uid"] = probeUid;
        QJsonArray probes = testObj["probes"].toArray();
        probes.append(probe);
        testObj["probes"] = probes;

        writeTableRec(dir.absoluteFilePath(testUid), testObj);

        //! Создание записи в таблице проб
        QDir dirProbe = probesDir();
        QJsonObject probeObj;
        probeObj["testUid"] = testUid;
        probeObj["name"] = name;
        probeObj["step"] = step;
        writeTableRec(dirProbe.absoluteFilePath(probeUid), probeObj);

        return probeUid;
    }

    return QString("");
}

void DataBase::addSignal(const QString &probeUid, const QString &channelUid, const QByteArray &data)
{
    //! uid нового сигнала
    auto signalUid = QUuid::createUuid().toString();

    //! Запись в данные пробы
    QDir dir = probesDir();
    QJsonObject probeObj;
    if (readTableRec(dir.absoluteFilePath(probeUid), probeObj))
    {
        QJsonObject signal;
        signal["uid"] = signalUid;
        signal["channelId"] = channelUid;
        QJsonArray signs = probeObj["channels"].toArray();
        signs.append(signal);
        probeObj["channels"] = signs;

        writeTableRec(dir.absoluteFilePath(probeUid), probeObj);

        //! Запись сигнала
        QDir dirSigns = channelsDir();
        QFile fileSign(dirSigns.absoluteFilePath(signalUid));
        if (fileSign.open(QIODevice::WriteOnly))
        {
            fileSign.write(data);
            fileSign.close();
        }
    }
}

QStringList DataBase::getTests() const
{
    QStringList retval;

    QDir dir = testsDir();
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    foreach (auto fileInfo, list)
        retval << fileInfo.fileName();
    return retval;
}

bool DataBase::getTestInfo(const QString &testUid, DataDefines::TestInfo &ti) const
{
    QDir dir = testsDir();
    QJsonObject testObj;
    if (readTableRec(dir.absoluteFilePath(testUid), testObj))
    {
        ti.uid = testUid;
        ti.patientUid = testObj["patientUid"].toString();
        ti.metodUid = testObj["metodUid"].toString();
        auto dt = testObj["datetime"].toString();
        ti.dateTime = QDateTime::fromString(dt, "dd.MM.yyyy hh:mm:ss");
        ti.params = testObj["params"].toObject();
        ti.comment = testObj["comment"].toString();
        ti.condition = testObj["condition"].toString();
        ti.isNormContained = testObj["norm_contained"].toBool();
        ti.isOpening = testObj["is_opening"].toBool();

        auto prbArr = testObj["probes"].toArray();
        ti.probes.clear();
        for (int i = 0; i < prbArr.size(); ++i)
        {
            auto obj = prbArr.at(i).toObject();
            ti.probes << obj["uid"].toString();
        }
        return true;
    }
    return false;
}

void DataBase::setTestProperty(const QString &testUid,
                               const QString &comment,
                               const QString &condition,
                               const bool isNorm)
{
    QDir dir = testsDir();
    QJsonObject testObj;
    if (readTableRec(dir.absoluteFilePath(testUid), testObj))
    {
        QString oldCondition = testObj["condition"].toString();
        bool oldIsNorm = testObj["norm_contained"].toBool();
        testObj["comment"] = comment;
        testObj["condition"] = condition;
        testObj["norm_contained"] = isNorm;

        writeTableRec(dir.absoluteFilePath(testUid), testObj);

        /*!
         Если по методике строятся нормы, то нужн контроллировать два параметра:
         - условия проведения
         - нормообразующий ли тест
         И при их изменении пересчитывать номы. Этим и займемся ниже
          */
        DataDefines::TestInfo ti;
        if (getTestInfo(testUid, ti))
        {
            MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->
                    getMetodics()->metodic(ti.metodUid);
            if (mi.buildNorms)
            {
                if (oldCondition == condition)
                {
                    //! Если не изменились условия, то пересчитаем норму, если изменился флаг нормообразующего
                    if (oldIsNorm != isNorm)
                        static_cast<AAnalyserApplication*>(QApplication::instance())->setTestNormContained(testUid, isNorm);
                }
                else
                {
                    //! Если изменились условия, то пересчитаем норму по любому, вне зависимости от состояния флага
                    static_cast<AAnalyserApplication*>(QApplication::instance())->setTestNormContained(testUid, isNorm);
                    //! Пересчет нормы по старым условиям
                    DataDefines::TestConditionInfo ci;
                    if (static_cast<AAnalyserApplication*>(QApplication::instance())->getTestConditionInfo(oldCondition, ci))
                    {
                        if (ci.norms_enabled)
                            static_cast<AAnalyserApplication*>(QApplication::instance())->
                                    calculatePersonalNorm(ti.patientUid, ti.metodUid, oldCondition);
                    }

                    //! Известим мир об изменении условий проведения у теста
                    emit changeTestCondition(testUid);
                }
            }
        }
    }
}

bool DataBase::isTestOpening(const QString &testUid)
{
    QDir dir = testsDir();
    QJsonObject testObj;
    if (readTableRec(dir.absoluteFilePath(testUid), testObj))
        return  testObj["is_opening"].toBool();
    return false;
}

void DataBase::setTestIsOpening(const QString &testUid, const bool isOpening)
{
    QDir dir = testsDir();
    QJsonObject testObj;
    if (readTableRec(dir.absoluteFilePath(testUid), testObj))
    {
        testObj["is_opening"] = isOpening;
        writeTableRec(dir.absoluteFilePath(testUid), testObj);
    }
}

bool DataBase::getProbeInfo(const QString &probeUid, DataDefines::ProbeInfo &pi) const
{
    QDir dir = probesDir();
    QJsonObject probeObj;
    if (readTableRec(dir.absoluteFilePath(probeUid), probeObj))
    {
        pi.uid = probeUid;
        pi.name = probeObj["name"].toString();
        pi.step = probeObj["step"].toInt();
        pi.testUid = probeObj["testUid"].toString();

        auto chansArr = probeObj["channels"].toArray();
        pi.channels.clear();
        for (int i = 0; i < chansArr.size(); ++i)
        {
            auto obj = chansArr.at(i).toObject();
            auto chanUid = obj["uid"].toString();
            auto channelId = obj["channelId"].toString();
            DataDefines::ChannelInfo chanInfo(chanUid, channelId);
            pi.channels << chanInfo;
        }
        return true;
    }
    return false;
}

bool DataBase::channelExists(const QString &probeUid, const QString &channelId) const
{
    DataDefines::ProbeInfo pi;
    if (getProbeInfo(probeUid, pi))
    {
        for (int i = 0; i < pi.channels.size(); ++i)
        {
            auto chanRec = pi.channels.at(i);
            if (chanRec.channelId == channelId)
                return channelExists(chanRec.uid);
        }
    }
    return false;
}

bool DataBase::channelExists(const QString &channelUid) const
{
    QDir dir = channelsDir();
    auto fileName = dir.absoluteFilePath(channelUid);
    return QFile::exists(fileName);
}

bool DataBase::channelExists(const QString &testUid, const int probeNum, const QString &channelId)
{
    DataDefines::TestInfo ti;
    if (getTestInfo(testUid, ti))
        if (probeNum >= 0 && probeNum < ti.probes.size())
            return channelExists(ti.probes.at(probeNum), channelId);
    return false;
}

bool DataBase::getChannel(const QString &probeUid, const QString &channelId, QByteArray &data) const
{
    QString uid = getChannelUid(probeUid, channelId);
    if (uid != QUuid().toString())
        return getChannel(uid, data);
    return false;
}

bool DataBase::getChannel(const QString &channelUid, QByteArray &data) const
{
    data.clear();

    QDir dir = channelsDir();
    auto fileName = dir.absoluteFilePath(channelUid);
    return readSignal(fileName, data);
//    if (QFile::exists(fileName))
//    {
//        QFile fChan(fileName);
//        if (fChan.open(QIODevice::ReadOnly))
//        {
//            data = fChan.readAll();
//            fChan.close();
//            return true;
//        }
//    }
    //    return false;
}

bool DataBase::setChannel(const QString &probeUid, const QString &channelId, QByteArray &data)
{
    QString uid = getChannelUid(probeUid, channelId);
    if (uid != QUuid().toString())
    {
        QDir dir = channelsDir();
        auto fileName = dir.absoluteFilePath(uid);
        bool isWriting = writeSignal(fileName, data);
        if (isWriting)
            static_cast<AAnalyserApplication*>(QApplication::instance())->doChannelChanged(probeUid, channelId);
        return isWriting;
    }
    return false;
}

QString DataBase::getChannelUid(const QString &probeUid, const QString &channelId) const
{
    DataDefines::ProbeInfo pi;
    if (getProbeInfo(probeUid, pi))
    {
        for (int i = 0; i < pi.channels.size(); ++i)
        {
            auto chanRec = pi.channels.at(i);
            if (chanRec.channelId == channelId)
                return chanRec.uid;
        }
    }
    return QUuid().toString();
}

bool DataBase::deleteTest(const QString &testUid) const
{
    DataDefines::TestInfo ti;
    if (getTestInfo(testUid, ti))
    {
        foreach (auto probeUid, ti.probes)
        {
            //! Удаление каналов
            DataDefines::ProbeInfo pi;
            if (getProbeInfo(probeUid, pi))
            {
                foreach (auto chan, pi.channels)
                {
                    QDir dir = channelsDir();
                    auto fileName = dir.absoluteFilePath(chan.uid);
                    if (QFile::exists(fileName))
                        QFile::remove(fileName);
                }
            }

            //! Удаление пробы
            QDir dir = probesDir();
            auto fileName = dir.absoluteFilePath(probeUid);
            if (QFile::exists(fileName))
                QFile::remove(fileName);
        }

        //! Удаление теста
        QDir dir = testsDir();
        auto fileName = dir.absoluteFilePath(testUid);
        if (QFile::exists(fileName))
            QFile::remove(fileName);
        return true;
    }
    return false;
}

QStringList DataBase::getProbesNames(const QString &testUid)
{
    QStringList retval;

    DataDefines::TestInfo ti;
    if (getTestInfo(testUid, ti))
    {
        foreach (auto probeId, ti.probes)
        {
            DataDefines::ProbeInfo pi;
            if (getProbeInfo(probeId, pi))
                retval << pi.name;
        }
    }

    return retval;
}

bool DataBase::createSection(QString &channelUid, QString &name, int channel, int from, int to, QByteArray &data)
{
    //! Определение имени файлов для новой секции
    QDir dir = channelsDir();
    int n = 1;
    QString fnData = "";
    QString fnCfg = "";
    QString fnNumber = "";
    do
    {
        fnNumber = QString::number(n).rightJustified(6, '0');
        fnData = dir.absoluteFilePath(channelUid + ".data." + fnNumber);
        fnCfg = dir.absoluteFilePath(channelUid + ".cfg." + fnNumber);
        ++n;
    }
    while (QFile::exists(fnData) || QFile::exists(fnCfg));

    //! Запись сигналов
    QFile fileData(fnData);
    if (fileData.open(QIODevice::WriteOnly))
    {
        fileData.write(data);
        fileData.close();
    }

    //! Запись параметров секции
    QJsonObject sectionObj;
    sectionObj["name"] = name;
    sectionObj["chan"] = channel;
    sectionObj["from"] = from;
    sectionObj["to"] = to;
    //sectionObj["actions"] = actions;
    writeTableRec(fnCfg, sectionObj);

    //! Известим мир о создании секции
    emit sectionCreated(channelUid, name, fnNumber);

    return true;
}

bool DataBase::getSections(const QString &channelUid, QList<DataDefines::SectionInfo> &sections)
{
    QDir dir = channelsDir();
    auto files = dir.entryList(QStringList() << channelUid + ".cfg.*");

    sections.clear();
    foreach (auto fn, files)
    {
        DataDefines::SectionInfo si;

        QJsonObject sectionObj;
        if (readTableRec(dir.absoluteFilePath(fn), sectionObj))
        {
            si.name = sectionObj["name"].toString();
            si.channel = sectionObj["chan"].toInt();
            si.from = sectionObj["from"].toInt();
            si.to = sectionObj["to"].toInt();
            si.actions = sectionObj["actions"].toArray();
            auto parts = fn.split('.');
            if (parts.size() == 3)
                si.number = parts.at(2);
            sections << si;
        }
    }

    return true;
}

bool DataBase::getSectionData(const QString &channelUid, const QString number, QByteArray &data)
{
    QDir dir = channelsDir();
    auto fileName = dir.absoluteFilePath(channelUid + ".data." + number);
    return readSignal(fileName, data);
}

bool DataBase::getSectionData(const QString &channelUid, const QString number, DataDefines::SectionInfo &si)
{
    QDir dir = channelsDir();
    auto fileName = dir.absoluteFilePath(channelUid + ".cfg." + number);

    QJsonObject sectionObj;
    if (readTableRec(fileName, sectionObj))
    {
        si.name = sectionObj["name"].toString();
        si.channel = sectionObj["chan"].toInt();
        si.from = sectionObj["from"].toInt();
        si.to = sectionObj["to"].toInt();
        si.actions = sectionObj["actions"].toArray();
        si.number = number;
        return true;
    }
    return false;
}

bool DataBase::updateSection(const QString &channelUid, const QString &number, QByteArray &data)
{
    //! Определение имени файлов для новой секции
    QDir dir = channelsDir();
    QString fnData = dir.absoluteFilePath(channelUid + ".data." + number);

    //! Запись сигналов
    QFile fileData(fnData);
    if (fileData.open(QIODevice::WriteOnly))
    {
        fileData.write(data);
        fileData.close();
    }

    return true;
}

bool DataBase::renameSection(const QString &channelUid, const QString &number, const QString &name)
{
    QDir dir = channelsDir();
    auto fileName = dir.absoluteFilePath(channelUid + ".cfg." + number);

    QJsonObject sectionObj;
    if (readTableRec(fileName, sectionObj))
    {
        sectionObj["name"] = name;
        writeTableRec(fileName, sectionObj);
        return true;
    }
    return false;
}

bool DataBase::deleteSection(const QString &channelUid, const QString &number)
{
    QDir dir = channelsDir();
    bool retval = true;

    QString fnData = dir.absoluteFilePath(channelUid + ".data." + number);
    if (QFile::exists(fnData))
        QFile::remove(fnData);
    else
        retval = false;

    QString fnCfg = dir.absoluteFilePath(channelUid + ".cfg." + number);
    if (QFile::exists(fnCfg))
        QFile::remove(fnCfg);
    else
        retval = false;

    return retval;
}

bool DataBase::addTransformActionToSection(const QString &channelUid, const QString &number, const QString &transformUid, QJsonObject &params)
{
    QDir dir = channelsDir();
    auto fileName = dir.absoluteFilePath(channelUid + ".cfg." + number);

    DataDefines::SectionInfo si;

    QJsonObject sectionObj;
    if (readTableRec(fileName, sectionObj))
    {
        auto actions = sectionObj["actions"].toArray();

        QJsonObject na;
        na["uid"] = transformUid;
        na["params"] = params;
        actions.append(na);
        sectionObj["actions"] = actions;

        writeTableRec(fileName, sectionObj);
    }

    return true;
}

bool DataBase::clearTransformActionToSection(const QString &channelUid, const QString &number)
{
    QDir dir = channelsDir();
    auto fileName = dir.absoluteFilePath(channelUid + ".cfg." + number);

    DataDefines::SectionInfo si;

    QJsonObject sectionObj;
    if (readTableRec(fileName, sectionObj))
    {
        sectionObj["actions"] = QJsonArray();
        writeTableRec(fileName, sectionObj);
    }

    return true;
}

void DataBase::addPrimaryFactor(const QString &testUid,
                                const QString &uid,
                                const double value,
                                const int probeNum,
                                const QString &channelId,
                                const QString &description)
{
    QDir dir = testsDir();
    QJsonObject testObj;
    if (readTableRec(dir.absoluteFilePath(testUid), testObj))
    {
        //! Поиск в массиве и замена значения, если нашли
        QJsonArray factors = testObj["factors"].toArray();
        for (int i = 0; i < factors.size(); ++i)
        {
            auto obj = factors.at(i).toObject();
            if (obj["uid"].toString() == uid &&
                obj["probeNum"].toInt() == probeNum &&
                obj["channelId"].toString() == channelId)
            {
                obj["value"] = value;
                factors.replace(i, obj);
                //! Запись в файл
                testObj["factors"] = factors;
                writeTableRec(dir.absoluteFilePath(testUid), testObj);
                return;
            }
        }

        //! Не нашли - создать новую запись
        QJsonObject obj;
        obj["uid"] = uid;
        obj["probeNum"] = probeNum;
        obj["channelId"] = channelId;
        obj["value"] = value;
        obj["description"] = description;
        factors.append(obj);

        //! Запись в файл
        testObj["factors"] = factors;
        writeTableRec(dir.absoluteFilePath(testUid), testObj);
    }
}

QList<FactorsDefines::FactorValueAdvanced> DataBase::getPrimaryFactors(const QString &testUid) const
{
    QList<FactorsDefines::FactorValueAdvanced> retval;
    retval.clear();

    QDir dir = testsDir();
    QJsonObject testObj;
    if (readTableRec(dir.absoluteFilePath(testUid), testObj))
    {
        QJsonArray factors = testObj["factors"].toArray();
        for (int i = 0; i < factors.size(); ++i)
        {
            auto obj = factors.at(i).toObject();
            FactorsDefines::FactorValueAdvanced factor(obj["uid"].toString(),
                    obj["value"].toDouble(),
                    obj["probeNum"].toInt(),
                    obj["channelId"].toString(),
                    obj["description"].toString());
            retval << factor;
        }
    }
    return retval;
}

void DataBase::setPersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                               const QString &factorUid, const int probeNum,
                               const double value, const double stdDeviation)
{
    QDir dir = personalNormDir();
    QString pnfn = personalNormFileName(patientUid, methodUid, conditionUid);
    QJsonObject pnObj;

    //! Чтение файла норм
    if (readTableRec(dir.absoluteFilePath(pnfn), pnObj))
    {
        //! Прочитали - разбираем массив норм
        QJsonArray norms = pnObj["norms"].toArray();
        for (int i = 0; i < norms.size(); ++i)
        {
            QJsonObject norm = norms.at(i).toObject();
            //! Если нашли норму, переписать значение и ско
            if (norm["factor"].toString() == factorUid &&
                norm["probe_num"].toInt() == probeNum)
            {
                norm["value"] = value;
                norm["std_deviation"] = stdDeviation;
                norms.replace(i, norm);
                pnObj["norms"] = norms;
                writeTableRec(dir.absoluteFilePath(pnfn), pnObj);
                return;
            }
        }
        //! Не нашли - создадим новую запись в файле
        QJsonObject norm;
        norm["factor"] = factorUid;
        norm["probe_num"] = probeNum;
        norm["value"] = value;
        norm["std_deviation"] = stdDeviation;
        norms.append(norm);
        pnObj["norms"] = norms;
        writeTableRec(dir.absoluteFilePath(pnfn), pnObj);
    }
    else
    {
        //! Не нашли файл - создать его и записать в него норму
        QJsonObject normsObj;
        QJsonArray norms;
        QJsonObject norm;
        norm["factor"] = factorUid;
        norm["probe_num"] = probeNum;
        norm["value"] = value;
        norm["std_deviation"] = stdDeviation;
        norms.append(norm);
        normsObj["norms"] = norms;
        writeTableRec(dir.absoluteFilePath(pnfn), normsObj);
    }
}

bool DataBase::personalNormExists(const QString &patientUid, const QString &methodUid, const QString &conditionUid)
{
    QDir dir = personalNormDir();
    QString pnfn = personalNormFileName(patientUid, methodUid, conditionUid);
    return QFile::exists(dir.absoluteFilePath(pnfn));
}

void DataBase::deletePersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid)
{
    QDir dir = personalNormDir();
    QString pnfn = personalNormFileName(patientUid, methodUid, conditionUid);
    if (QFile::exists(dir.absoluteFilePath(pnfn)))
        QFile::remove(dir.absoluteFilePath(pnfn));
}

bool DataBase::getPersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                               QList<DataDefines::PersonalNormInfo> &pnil) const
{
    pnil.clear();

    QDir dir = personalNormDir();
    QString pnfn = personalNormFileName(patientUid, methodUid, conditionUid);
    QJsonObject pnObj;

    //! Чтение файла норм
    if (readTableRec(dir.absoluteFilePath(pnfn), pnObj))
    {
        //! Прочитали - разбираем массив норм
        QJsonArray norms = pnObj["norms"].toArray();
        for (int i = 0; i < norms.size(); ++i)
        {
            QJsonObject norm = norms.at(i).toObject();

            //! Элемент массива - в выходной список
            DataDefines::PersonalNormInfo pni;
            pni.patientUid = patientUid;
            pni.methodicUid = methodUid;
            pni.conditionUid = conditionUid;
            pni.factorUid = norm["factor"].toString();
            pni.probeNum = norm["probe_num"].toInt();
            pni.value = norm["value"].toDouble();
            pni.stdDeviation = norm["std_deviation"].toDouble();
            pnil << pni;
        }

        return norms.size() > 0;
    }
    return false;
}

QStringList DataBase::getTests(const QString &patientUid, const QString &methodUid, const QString &conditionUid) const
{
    QStringList retval;

    QDir dir = testsDir();
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    foreach (auto fileInfo, list)
    {
        DataDefines::TestInfo ti;
        if (getTestInfo(fileInfo.fileName(), ti))
            if (ti.patientUid == patientUid && ti.metodUid == methodUid && ti.condition == conditionUid)
                retval << fileInfo.fileName();
    }
    return retval;
}

void DataBase::setTestNormContained(const QString &testUid, const bool isNormContained)
{
    QDir dir = testsDir();
    QJsonObject testObj;
    if (readTableRec(dir.absoluteFilePath(testUid), testObj))
    {
        testObj["norm_contained"] = isNormContained;
        writeTableRec(dir.absoluteFilePath(testUid), testObj);
    }
}

void DataBase::assignPersonalProgramForPatient(const QString &uidPPAssigned, const QJsonObject &pp)
{
    DataDefines::PatientKard pi;

    if (getPatient(pp["patient_uid"].toString(), pi))
    {
        pi.pp_uid = uidPPAssigned;
        updatePatient(pi);

        //! Запись ИП в таблицу ИП. Проверка, не назначена ли активная ИП для этого пациента
        QDir dir = personalProgramsDir();
        QString pnfn = uidPPAssigned;
        writeTableRec(dir.absoluteFilePath(pnfn), pp);
    }
}

QJsonObject DataBase::getActivePersonalProgramForPatient(const QString &patientUid)
{
    QDir dir = personalProgramsDir();
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
    {
        QJsonObject retval;
        if (readTableRec(dir.absoluteFilePath(fileInfo.fileName()), retval))
        {
            if (retval["patient_uid"].toString() == patientUid &&
                retval["active"].toBool() == true)
                return retval;
        }
    }
    return QJsonObject();
}

QJsonObject DataBase::getPersonalProgramByUid(const QString &uidPP)
{
    QDir dir = personalProgramsDir();
    if (QFile::exists(dir.absoluteFilePath(uidPP)))
    {
        QJsonObject retval;
        if (readTableRec(dir.absoluteFilePath(uidPP), retval))
        {
//            retval["assigned_uid"] = uidPP;
            return retval;
        }
    }
    return QJsonObject();
}

void DataBase::savePersonalProgramByUid(const QString &uidPP, const QJsonObject &objPP)
{
    QDir dir = personalProgramsDir();
    QString pnfn = uidPP;
    writeTableRec(dir.absoluteFilePath(pnfn), objPP);
}

QJsonArray DataBase::getPersonalProgramListForPatient(const QString &patientUid)
{
    QJsonArray retval;
    QDir dir = personalProgramsDir();
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
    {
        QJsonObject obj;
        if (readTableRec(dir.absoluteFilePath(fileInfo.fileName()), obj))
        {
            if (obj["patient_uid"].toString() == patientUid)
                retval << obj;
        }
    }
    return retval;
}

QJsonArray DataBase::getPersonalProgramList()
{
    QJsonArray retval;
    QDir dir = personalProgramsDir();
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
    {
        QJsonObject obj;
        if (readTableRec(dir.absoluteFilePath(fileInfo.fileName()), obj))
        {
            //obj["assigned_uid"] = fileInfo.fileName();  //! Добавляем uid назначенной ИП, ибо иначе данные о нем тут теряются
            retval << obj;
        }
    }
    return retval;
}

void DataBase::deletePersonalProgramForPatient(const QString &ppUid)
{
    QDir dir = personalProgramsDir();
    if (QFile::exists(dir.absoluteFilePath(ppUid)))
        QFile::remove(dir.absoluteFilePath(ppUid));
}

void DataBase::deactivatePersonalProgramForPatient(const QString &ppUid)
{
    QDir dir = personalProgramsDir();
    if (QFile::exists(dir.absoluteFilePath(ppUid)))
    {
        QJsonObject objPP;
        if (readTableRec(dir.absoluteFilePath(ppUid), objPP))
        {
            objPP["active"] = false;
            objPP["date_end"] = QDateTime::currentDateTime().toString("dd.MM.yyyy");
            writeTableRec(dir.absoluteFilePath(ppUid), objPP);
        }
    }
}

void DataBase::clear()
{
    disconnected();
    deleteAllTests();
    QDir dir = patientsDir();
    clearDBFolder(dir);
    connected();
}

void DataBase::deleteTests()
{
    disconnected();
    deleteAllTests();
    connected();
}

void DataBase::exportBD(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);
        if (QFile::exists(currentDataBase() + "db.info"))
            addFileToByteArray(currentDataBase() + "db.info", "db.info", stream);

        //! Обработка папки БД
        auto scanFolder = [&](QDir &dir)
        {
            QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
            int count = list.size();
            stream << count;
            foreach (auto fileInfo, list)
                addFileToByteArray(dir.absoluteFilePath(fileInfo.fileName()), fileInfo.fileName(), stream);
        };

        //! Проход по папкам БД
        QDir dbDir(currentDataBase());
        QFileInfoList list = dbDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        stream << list.size();
        foreach (auto fileInfo, list)
        {
            stream << fileInfo.fileName();
            QDir dir = currentDataBase() + fileInfo.fileName() + "/";
            scanFolder(dir);
        }

        file.close();
    }
}

void DataBase::importBD(const QString &fileName)
{
    QFile fileRec(fileName);
    if (fileRec.open(QIODevice::ReadOnly))
    {
        QString dbName = DataDefines::dataBasesPath() + getNewDatabaseName() + "/";
        QDir dir(dbName);
        if (!dir.exists())
            dir.mkpath(dbName);

        QDataStream stream(&fileRec);

        //! Создание файла в БД
        auto createFile = [&](QString &folder)
        {
            QString fn = "";
            stream >> fn;
            QByteArray ba;
            stream >> ba;

            QFile file(folder + fn);
            if (file.open(QIODevice::WriteOnly))
            {
                file.write(ba);
                file.close();
            }
        };

        //! Обработка папки БД
        auto processDBFolder = [&](QString &folder)
        {
            QDir dir(folder);
            dir.mkpath(folder);
            int count = 0;
            stream >> count;
            for (int i = 0; i < count; ++i)
                createFile(folder);
        };

        //! db.info
        createFile(dbName);

        //! Проход по папкам БД
        int fldrCount = 0;
        stream >> fldrCount;

        for (int i = 0; i < fldrCount; ++i)
        {
            QString fldrName = "";
            stream >> fldrName;
            QString dbFolder = dbName + fldrName + "/";
            processDBFolder(dbFolder);
        }

        changeDatabase(dbName);
    }
}

void DataBase::changeDatabase(const QString &dataBaseFolder)
{
    emit disconnected();
    SettingsProvider::setValueToRegAppCopy("Database", "path", dataBaseFolder);
    QTimer::singleShot(0, [=]()
    {
        emit connected();
    });
}

void DataBase::createDatabase()
{
    QString name = getNewDatabaseName();
    changeDatabase(DataDefines::dataBasesPath() + name + "/");
}

QString DataBase::currentDataBase() const
{
    auto path = SettingsProvider::valueFromRegAppCopy("Database", "path", QString("")).toString();

    if (path != "")
        return path;
    else
    {
        int i = 0;
        bool exists = false;
        do
        {
            if (i == 0)
                path = DataDefines::dataBasesPath() + m_dataBaseNameDef + "/";
            else
                path = DataDefines::dataBasesPath() + m_dataBaseNameDef + " " + QString::number(i) + "/";
            QDir dir (path);
            exists = dir.exists();
            ++i;
        }
        while (exists);
        SettingsProvider::setValueToRegAppCopy("Database", "path", path);
        return path;
    }
}

QDir DataBase::patientsDir() const
{
    return localDir("patients");
}

QDir DataBase::testsDir() const
{
    return localDir("tests");
}

QDir DataBase::probesDir() const
{
    return localDir("probes");
}

QDir DataBase::channelsDir() const
{
    return localDir("channels");
}

QDir DataBase::personalNormDir() const
{
    return localDir("personal_norms");
}

QDir DataBase::personalProgramsDir() const
{
    return localDir("personal_programs");
}

QDir DataBase::localDir(const QString &dirName) const
{
    QDir dir(currentDataBase() + dirName + "/");
    if (!dir.exists())
        dir.mkpath(currentDataBase() + dirName + "/");
    return dir;
}

QString DataBase::personalNormFileName(const QString &patientUid, const QString &methodicUid, const QString &conditionUid) const
{
    return patientUid + "_" + methodicUid + "_" + conditionUid;
}


QString DataBase::createPatientRec(const DataDefines::PatientKard patient)
{
    QString uid = patient.uid;
    if (uid == "")
        uid = QUuid::createUuid().toString();

    QDir dir = patientsDir();
    QFile fPatientRec(dir.absoluteFilePath(uid));
    if (fPatientRec.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject root;
        root["fio"] = patient.fio;
        root["born"] = patient.born.toString("dd.MM.yyyy");
        root["sex"] = patient.sex;
        root["massa"] = patient.massa;
        root["height"] = patient.height;
        root["personal_program"] = patient.pp_uid;
        QJsonDocument doc(root);
        QByteArray ba = doc.toJson();
        fPatientRec.write(ba);

        fPatientRec.close();
    }
    return uid;
}

bool DataBase::readTableRec(const QString &fullFileName, QJsonObject &rec) const
{
    QFile fileRec(fullFileName);
    if (fileRec.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fileRec.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        rec = loadDoc.object();
        fileRec.close();
        return true;
    }
    return false;
}

bool DataBase::writeTableRec(const QString &fullFileName, const QJsonObject &rec) const
{
    QFile fileRec(fullFileName);
    fileRec.remove();
    if (fileRec.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument doc(rec);
        QByteArray ba = doc.toJson();
        fileRec.write(ba);

        fileRec.close();
        return true;
    }
    return false;
}

bool DataBase::readSignal(const QString& fileName, QByteArray &data) const
{
    if (QFile::exists(fileName))
    {
        QFile fChan(fileName);
        if (fChan.open(QIODevice::ReadOnly))
        {
            data = fChan.readAll();
            fChan.close();
            return true;
        }
    }
    return false;
}

bool DataBase::writeSignal(const QString &fileName, QByteArray &data) const
{
    QFile fChan(fileName);
    if (fChan.open(QIODevice::WriteOnly))
    {
        fChan.write(data);
        fChan.close();
        return true;
    }
    return false;
}

void DataBase::updatePatientRec(const DataDefines::PatientKard &patient)
{
    QDir dir = patientsDir();
    QJsonObject patObj;
    if (readTableRec(dir.absoluteFilePath(patient.uid), patObj))
    {
        patObj["fio"] = patient.fio;
        patObj["born"] = patient.born.toString("dd.MM.yyyy");
        patObj["sex"] = patient.sex;
        patObj["massa"] = patient.massa;
        patObj["height"] = patient.height;
        patObj["personal_program"] = patient.pp_uid;

        writeTableRec(dir.absoluteFilePath(patient.uid), patObj);
    }
}

bool DataBase::patientExists(const QString &uid) const
{
    QDir dir = patientsDir();
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() == uid)
            return true;
    return false;
}

QString DataBase::getNewDatabaseName()
{
    bool dbExists = false;
    int n = 0;
    QString name = "data";
    do
    {
        if (n > 0)
            name = "data " + QString::number(n);
        QDir dir(DataDefines::dataBasesPath() + name);
        dbExists = dir.exists();
        ++n;
    }
    while (dbExists);
    return name;
}

void DataBase::clearDBFolder(QDir &dir)
{
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    foreach (auto fileInfo, list)
        QFile::remove(fileInfo.absoluteFilePath());
}

void DataBase::deleteAllTests()
{
    QDir dbDir(currentDataBase());
    QFileInfoList list = dbDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
    {
        if (fileInfo.fileName() != "patients")
        {
            QDir dir(currentDataBase() + fileInfo.fileName() + "/");
            clearDBFolder(dir);
        }
    }
}

void DataBase::addFileToByteArray(const QString &fullFileName, const QString &fileName, QDataStream &stream)
{
    QFile file(fullFileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
//        int s = baf.size();

//        stream << s;
        stream << fileName;
        stream << ba;
    }
}
