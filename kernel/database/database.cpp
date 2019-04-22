#include "database.h"

#include "datadefines.h"

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

QStringList DataBase::getPatients()
{
    QStringList retval;

    QDir dir = patientsDir();
    QFileInfoList list = dir.entryInfoList();
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
        retval << fileInfo.fileName();
    return retval;
}

bool DataBase::getPatient(const QString &uid, DataDefines::PatientKard &patient)
{
    DataDefines::PatientKard kard;
    QDir dir = patientsDir();
    readPatientRec(dir.absoluteFilePath(uid), kard);

    patient.uid = uid;
    patient.fio = kard.fio;
    patient.born = kard.born;
    patient.sex = kard.sex;
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

QString DataBase::currentDataBase() const
{
    return DataDefines::dataBasesPath() + m_dataBaseName + "/";
}

QDir DataBase::patientsDir() const
{
    QDir dir(currentDataBase() + "patients/");
    if (!dir.exists())
        dir.mkpath(currentDataBase() + "patients/");
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

bool DataBase::readPatientRec(const QString &fullFileName, DataDefines::PatientKard &patient)
{
    QFile fPatient(fullFileName);
    if (fPatient.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fPatient.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        QJsonObject root = loadDoc.object();

        patient.fio = root["fio"].toString();
        patient.born = QDate::fromString(root["born"].toString(), "dd.MM.yyyy");
        patient.sex = static_cast<DataDefines::Sex>(root["sex"].toInt());

        fPatient.close();
    }
}

void DataBase::updatePatientRec(const DataDefines::PatientKard &patient)
{
    QDir dir = patientsDir();
    QFile fPatientRec(dir.absoluteFilePath(patient.uid));
    fPatientRec.remove();
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

bool DataBase::patientExists(const QString &uid) const
{
    QDir dir = patientsDir();
    QFileInfoList list = dir.entryInfoList();
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() == uid)
            return true;
    return false;
}
