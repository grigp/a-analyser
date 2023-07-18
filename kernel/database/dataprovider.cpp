#include "dataprovider.h"

#include <QApplication>
#include <QDebug>

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

QString DataProvider::updatePatient(const DataDefines::PatientKard &patient)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->updatePatient(patient);
}

QVariant DataProvider::patientData(const QString &uid, const QString &name)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->patientData(uid, name);
}

void DataProvider::patientSetData(const QString &uid, const QString &name, const QVariant value)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->patientSetData(uid, name, value);
}

void DataProvider::removePatient(const QString &uid)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->removePatient(uid);
}

QList<MetodicDefines::MetodicInfo> DataProvider::getListMetodics()
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

/*!
 * \brief Вызывает диалог редактирования параметров методики по uid методики
 * \param parent - родительский виджет для вызываемого диалогового окна
 * \param metUid - uid методики
 * \param params - редактируемые параметры методики
 * \return true, если в диалоге нажали OK и параметры надо сохранить
 */
bool DataProvider::editMetodicParams(QWidget *parent, const QString &metUid, QJsonObject &params)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->
            editMetodicParams(parent, metUid, params);
}

void DataProvider::execute(QWidget *parent, const QString &metUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->execute(parent, metUid);
}

QList<MetodicDefines::MetodicKindInfo> DataProvider::getListMetodicKinds()
{
    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();

    QList<MetodicDefines::MetodicKindInfo> retval;
    for (int i = 0; i < metFactory->metodicKindsCount(); ++i)
        retval << metFactory->metodicKind(i);
    return retval;
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

void DataProvider::setTestProperty(const QString &testUid,
                                   const QString &comment,
                                   const QString &condition,
                                   const bool isNorm)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            setTestProperty(testUid, comment, condition, isNorm);
}

bool DataProvider::isTestOpening(const QString &testUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            isTestOpening(testUid);
}
void DataProvider::setTestIsOpening(const QString &testUid, const bool isOpening)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            setTestIsOpening(testUid, isOpening);
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

bool DataProvider::setChannel(const QString &probeUid, const QString &channelId, QByteArray &data)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->setChannel(probeUid, channelId, data);
}

QString DataProvider::getChannelUid(const QString &probeUid, const QString &channelId)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getChannelUid(probeUid, channelId);
}

QStringList DataProvider::getProbesNames(const QString &testUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getProbesNames(testUid);
}

bool DataProvider::createSection(QString &channelUid, QString &name, int channel, int from, int to, QByteArray &data)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->createSection(channelUid, name, channel, from, to, data);
}

bool DataProvider::getSections(const QString &channelUid, QList<DataDefines::SectionInfo> &sections)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getSections(channelUid, sections);
}

bool DataProvider::getSectionData(const QString &channelUid, const QString number, QByteArray &data)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getSectionData(channelUid, number, data);
}

bool DataProvider::getSectionData(const QString &channelUid, const QString number, DataDefines::SectionInfo &si)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getSectionData(channelUid, number, si);
}

bool DataProvider::updateSection(const QString &channelUid, const QString &number, QByteArray &data)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->updateSection(channelUid, number, data);
}

bool DataProvider::renameSection(const QString &channelUid, const QString &number, const QString &name)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->renameSection(channelUid, number, name);
}

bool DataProvider::deleteSection(const QString &channelUid, const QString &number)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->deleteSection(channelUid, number);
}

bool DataProvider::addTransformActionToSection(const QString &channelUid, const QString &number, const QString &transformUid, QJsonObject &params)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->addTransformActionToSection(channelUid, number, transformUid, params);
}

bool DataProvider::clearTransformActionToSection(const QString &channelUid, const QString &number)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->clearTransformActionToSection(channelUid, number);
}

bool DataProvider::channelExists(const QString &probeUid, const QString &channelId)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->channelExists(probeUid, channelId);
}

bool DataProvider::channelExists(const QString &channelUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->channelExists(channelUid);
}

bool DataProvider::channelExists(const QString &testUid, const int probeNum, const QString &channelId)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->channelExists(testUid, probeNum, channelId);
}

void DataProvider::addPrimaryFactor(const QString &testUid,
                                    const QString &uid,
                                    const double value,
                                    const int probeNum,
                                    const QString &channelId,
                                    const QString &description)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            addPrimaryFactor(testUid, uid, value, probeNum, channelId, description);
}

QList<FactorsDefines::FactorValueAdvanced> DataProvider::getPrimaryFactors(const QString &testUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->getPrimaryFactors(testUid);
}

void DataProvider::setPersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                                   const QString &factorUid, const int probeNum,
                                   const double value, const double stdDeviation)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            setPersonalNorm(patientUid, methodUid, conditionUid, factorUid, probeNum, value, stdDeviation);
}

bool DataProvider::getPersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                                   QList<DataDefines::PersonalNormInfo> &pnil)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            getPersonalNorm(patientUid, methodUid, conditionUid, pnil);
}

bool DataProvider::personalNormExists(const QString &patientUid, const QString &methodUid, const QString &conditionUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            personalNormExists(patientUid, methodUid, conditionUid);
}

void DataProvider::deletePersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            deletePersonalNorm(patientUid, methodUid, conditionUid);
}

QStringList DataProvider::getTests(const QString &patientUid, const QString &methodUid, const QString &conditionUid)
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            getTests(patientUid, methodUid, conditionUid);
}

void DataProvider::setTestNormContained(const QString &testUid, const bool isNormContained)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->
            setTestNormContained(testUid, isNormContained);
}

void DataProvider::assignPersonalProgramForPatient(const QString& uidPPAssigned, const QJsonObject& pp)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->assignPersonalProgramForPatient(uidPPAssigned, pp);
}

void DataProvider::clear()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->clear();
}

void DataProvider::deleteTests()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->deleteTests();
}

void DataProvider::exportBD(const QString &fileName)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->exportBD(fileName);
}

void DataProvider::importBD(const QString &fileName)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getDB()->importBD(fileName);
}


