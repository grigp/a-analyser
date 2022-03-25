#include "dataprovider.h"

#include <QApplication>
#include <QDebug>

#include "kerdoapplication.h"
#include "database.h"


QStringList DataProvider::getPatients()
{
    return static_cast<KerdoApplication*>(QApplication::instance())->getDB()->getPatients();
}


bool DataProvider::getPatient(const QString &uid, DataDefines::PatientKard &patient)
{
    return static_cast<KerdoApplication*>(QApplication::instance())->getDB()->getPatient(uid, patient);
}

QString DataProvider::updatePatient(const DataDefines::PatientKard &patient)
{
    return static_cast<KerdoApplication*>(QApplication::instance())->getDB()->updatePatient(patient);
}

void DataProvider::removePatient(const QString &uid)
{
    static_cast<KerdoApplication*>(QApplication::instance())->getDB()->removePatient(uid);
}

void DataProvider::clear()
{
    static_cast<KerdoApplication*>(QApplication::instance())->getDB()->clear();
}

void DataProvider::deleteTests()
{
    static_cast<KerdoApplication*>(QApplication::instance())->getDB()->deleteTests();
}

void DataProvider::exportBD(const QString &fileName)
{
    static_cast<KerdoApplication*>(QApplication::instance())->getDB()->exportBD(fileName);
}

void DataProvider::importBD(const QString &fileName)
{
    static_cast<KerdoApplication*>(QApplication::instance())->getDB()->importBD(fileName);
}



QString DataProvider::addTest(const QString &uidPatient)
{
    return static_cast<KerdoApplication*>(QApplication::instance())->getDB()->addTest(uidPatient);
}

QStringList DataProvider::getTests(const QString &uidPatient)
{
    return static_cast<KerdoApplication*>(QApplication::instance())->getDB()->getTests(uidPatient);
}

bool DataProvider::getTest(const QString &uidPatient, const QString &uidTest, DataDefines::Result &test)
{
    return static_cast<KerdoApplication*>(QApplication::instance())->getDB()->getTest(uidPatient, uidTest, test);
}

void DataProvider::setTestResult(const QString &uidPatient, const QString &uidTest, const DataDefines::Result &test)
{
    static_cast<KerdoApplication*>(QApplication::instance())->getDB()->setTestResult(uidPatient, uidTest, test);
}
