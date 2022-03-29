#include "database.h"

#include "datadefines.h"
#include "settingsprovider.h"

#include <QJsonDocument>
#include <QDataStream>
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

        auto tests = patObj["tests"].toArray();
        for (int i = 0; i < tests.size(); ++i)
        {
            auto test = tests[i].toObject();

            DataDefines::Result result;
            result.uid = test["uid"].toString();
            result.dateTime = QDateTime::fromString(test["datetime"].toString(), "dd.MM.yyyy");
            result.beforePulse = test["pulse_before"].toInt(0);
            result.beforeDAD = test["dad_before"].toInt(0);
            result.beforeKerdo = test["kerdo_before"].toDouble(0);
            result.afterPulse = test["pulse_after"].toInt(0);
            result.afterDAD = test["dad_after"].toInt(0);
            result.afterKerdo = test["kerdo_after"].toDouble(0);
            patient.results << result;
        }

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

void DataBase::removePatient(const QString &uid)
{
    if (patientExists(uid))
    {
//        auto tests = getTests();
//        foreach (auto testUid, tests)
//        {
//            DataDefines::TestInfo ti;
//            if (getTestInfo(testUid, ti))
//                if (ti.patientUid == uid)
//                    static_cast<AAnalyserApplication*>(QApplication::instance())->deleteTest(testUid);
//        }

        QDir dir = patientsDir();
        QFile fPatientRec(dir.absoluteFilePath(uid));
        fPatientRec.remove();
    }
}

QString DataBase::addTest(const QString &uidPatient)
{
    QDir dir = patientsDir();
    QJsonObject patObj;
    if (readTableRec(dir.absoluteFilePath(uidPatient), patObj))
    {
        QJsonObject newTest;
        auto uid = QUuid::createUuid().toString();
        newTest["uid"] = uid;
        newTest["datetime"] = QDateTime::currentDateTime().toString("dd.MM.yyyy");
        newTest["pulse_before"] = 0;
        newTest["dad_before"] = 0;
        newTest["kerdo_before"] = 0;
        newTest["pulse_after"] = 0;
        newTest["dad_after"] = 0;
        newTest["kerdo_after"] = 0;

        auto tests = patObj["tests"].toArray();
        tests.append(newTest);

        patObj["tests"] = tests;
        writeTableRec(dir.absoluteFilePath(uidPatient), patObj);

        return uid;
    }
    return "";
}

QStringList DataBase::getTests(const QString &uidPatient)
{
    QDir dir = patientsDir();
    QJsonObject patObj;
    if (readTableRec(dir.absoluteFilePath(uidPatient), patObj))
    {
        auto tests = patObj["tests"].toArray();
        QStringList retval;
        retval.clear();
        for (int i = 0; i < tests.size(); ++i)
            retval << tests.at(i).toObject()["uid"].toString();
        return retval;
    }
    return QStringList();
}

bool DataBase::getTest(const QString &uidPatient, const QString &uidTest, DataDefines::Result &test)
{
    QDir dir = patientsDir();
    QJsonObject patObj;
    if (readTableRec(dir.absoluteFilePath(uidPatient), patObj))
    {
        auto tests = patObj["tests"].toArray();
        for (int i = 0; i < tests.size(); ++i)
        {
            auto tst = tests.at(i).toObject();
            if (tst["uid"].toString() == uidTest)
            {
                test.uid = tst["uid"].toString();
                test.dateTime = QDateTime::fromString(tst["datetime"].toString(), "dd.MM.yyyy");
                test.beforePulse = tst["pulse_before"].toInt();
                test.beforeDAD = tst["dad_before"].toInt();
                test.beforeKerdo = tst["kerdo_before"].toDouble();
                test.afterPulse = tst["pulse_after"].toInt();
                test.afterDAD = tst["dad_after"].toInt();
                test.afterKerdo = tst["kerdo_after"].toDouble();
                return true;
            }
        }
    }
    return false;
}

void DataBase::setTestResult(const QString &uidPatient, const QString &uidTest, const DataDefines::Result &test)
{
    QDir dir = patientsDir();
    QJsonObject patObj;
    if (readTableRec(dir.absoluteFilePath(uidPatient), patObj))
    {
        auto tests = patObj["tests"].toArray();
        for (int i = 0; i < tests.size(); ++i)
        {
            auto tst = tests.at(i).toObject();
            if (tst["uid"].toString() == uidTest)
            {
                tst["pulse_before"] = test.beforePulse;
                tst["dad_before"] = test.beforeDAD;
                tst["kerdo_before"] = test.beforeKerdo;
                tst["pulse_after"] = test.afterPulse;
                tst["dad_after"] = test.afterDAD;
                tst["kerdo_after"] = test.afterKerdo;

                tests.replace(i, tst);
                patObj["tests"] = tests;
                writeTableRec(dir.absoluteFilePath(uidPatient), patObj);

                return;
            }
        }
    }
}

void DataBase::clear()
{
    disconnected();
    //deleteAllTests();
    QDir dir = patientsDir();
    clearDBFolder(dir);
    connected();
}

void DataBase::deleteTests()
{
    disconnected();
//    deleteAllTests();
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

QDir DataBase::localDir(const QString &dirName) const
{
    QDir dir(currentDataBase() + dirName + "/");
    if (!dir.exists())
        dir.mkpath(currentDataBase() + dirName + "/");
    return dir;
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
    QString name = m_dataBaseNameDef;
    do
    {
        if (n > 0)
            name = m_dataBaseNameDef + " " + QString::number(n);
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
