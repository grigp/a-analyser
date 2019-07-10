#include "database.h"

#include "datadefines.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"

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
    QFileInfoList list = dir.entryInfoList();
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
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

QString DataBase::addProbe(const QString &testUid, const int step)
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
    QFileInfoList list = dir.entryInfoList();
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
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

bool DataBase::getProbeInfo(const QString &probeUid, DataDefines::ProbeInfo &pi) const
{
    QDir dir = probesDir();
    QJsonObject probeObj;
    if (readTableRec(dir.absoluteFilePath(probeUid), probeObj))
    {
        pi.uid = probeUid;
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

QString DataBase::currentDataBase() const
{
    return DataDefines::dataBasesPath() + m_dataBaseName + "/";
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

QDir DataBase::localDir(const QString &dirName) const
{
    QDir dir(currentDataBase() + dirName + "/");
    if (!dir.exists())
        dir.mkpath(currentDataBase() + dirName + "/");
    return dir;
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
    QFileInfoList list = dir.entryInfoList();
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() == uid)
            return true;
    return false;
}
