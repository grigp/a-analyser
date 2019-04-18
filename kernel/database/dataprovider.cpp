#include "dataprovider.h"

#include <QApplication>
#include "aanalyserapplication.h"
#include "database.h"


QStringList DataProvider::getPatients()
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getPatients();
}


bool DataProvider::getPatient(const QString &uid, DataDefines::PatientKard &patient)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getPatient(uid, patient);
}

void DataProvider::updatePatient(const DataDefines::PatientKard &patient)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->updatePatient(patient);
}
