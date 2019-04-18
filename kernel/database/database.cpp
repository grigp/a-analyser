#include "database.h"

#include "datadefines.h"

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
    return QStringList();
}

bool DataBase::getPatient(const QString &uid, DataDefines::PatientKard &patient)
{
    patient.fio = "";
    patient.sex = DataDefines::male;
}

void DataBase::updatePatient(const DataDefines::PatientKard &patient)
{

}
