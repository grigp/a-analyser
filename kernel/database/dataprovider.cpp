#include "dataprovider.h"

#include <QApplication>
#include "aanalyserapplication.h"
#include "database.h"
#include "metodicsfactory.h"


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
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->updatePatient(patient);
}

void DataProvider::removePatient(const QString &uid)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->removePatient(uid);
}

QList<MetodicDefines::MetodicInfo> DataProvider::getListMetodisc()
{
    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();

    QList<MetodicDefines::MetodicInfo> retval;
    for (int i = 0; i < metFactory->metodicsCount(); ++i)
        retval << metFactory->metodic(i);
    return retval;
}

bool DataProvider::editMetodicParams(QWidget *parent, const QString &metUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->
            editMetodicParams(parent, metUid);
}

void DataProvider::execute(QWidget *parent, const QString &metUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->execute(parent, metUid);
}

QString DataProvider::addTestStart(const QString &patientUid, const QString &metodUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->addTestStart(patientUid, metodUid);
}

void DataProvider::addTestFinish(const QString &testUid)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->addTestFinish(testUid);
}

QString DataProvider::addProbe(const QString &testUid, const int step, const QString &name)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->addProbe(testUid, step, name);
}

void DataProvider::addSignal(const QString &probeUid, const QString &channelUid, const QByteArray &data)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->addSignal(probeUid, channelUid, data);
}

QStringList DataProvider::getTests()
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getTests();
}

bool DataProvider::getTestInfo(const QString &testUid, DataDefines::TestInfo &ti)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getTestInfo(testUid, ti);
}


bool DataProvider::getProbeInfo(const QString &probeUid, DataDefines::ProbeInfo &pi)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getProbeInfo(probeUid, pi);
}


bool DataProvider::getChannel(const QString &probeUid, const QString &channelId, QByteArray &data)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getChannel(probeUid, channelId, data);
}

bool DataProvider::getChannel(const QString &channelUid, QByteArray &data)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getChannel(channelUid, data);
}

bool DataProvider::channelExists(const QString &probeUid, const QString &channelId)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->channelExists(probeUid, channelId);
}

bool DataProvider::channelExists(const QString &channelUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->channelExists(channelUid);
}
