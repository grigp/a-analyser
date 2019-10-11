#include "database.h"

#include "datadefines.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "settingsprovider.h"

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
        return true;
    }
    return false;
}

void DataBase::updatePatient(const DataDefines::PatientKard &patient)
{
    if (patient.uid == "" || !patientExists(patient.uid))
        createPatientRec(patient);
    else
        updatePatientRec(patient);
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
        testObj["comment"] = comment;
        testObj["condition"] = condition;
        testObj["norm_contained"] = isNorm;

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

bool DataBase::getChannel(const QString &probeUid, const QString &channelId, QByteArray &data) const
{
    DataDefines::ProbeInfo pi;
    if (getProbeInfo(probeUid, pi))
    {
        for (int i = 0; i < pi.channels.size(); ++i)
        {
            auto chanRec = pi.channels.at(i);
            if (chanRec.channelId == channelId)
                return getChannel(chanRec.uid, data);
        }
    }
    return false;
}

bool DataBase::getChannel(const QString &channelUid, QByteArray &data) const
{
    data.clear();

    QDir dir = channelsDir();
    auto fileName = dir.absoluteFilePath(channelUid);
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


void DataBase::createPatientRec(const DataDefines::PatientKard patient)
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
        QJsonDocument doc(root);
        QByteArray ba = doc.toJson();
        fPatientRec.write(ba);

        fPatientRec.close();
    }
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

void DataBase::updatePatientRec(const DataDefines::PatientKard &patient)
{
    QDir dir = patientsDir();
    QJsonObject patObj;
    if (readTableRec(dir.absoluteFilePath(patient.uid), patObj))
    {
        patObj["fio"] = patient.fio;
        patObj["born"] = patient.born.toString("dd.MM.yyyy");
        patObj["sex"] = patient.sex;

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
