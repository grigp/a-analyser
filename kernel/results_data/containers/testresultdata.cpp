#include "testresultdata.h"

#include "proberesultdata.h"
#include "resultinfo.h"

#include <QUuid>

TestResultData::TestResultData()
{

}

void TestResultData::newTest(const QString &patient, const QString &metodicName)
{
    Q_ASSERT(m_mode == mdUndefined);
    m_mode = mdRun;

    m_uid = QUuid::createUuid().toString();
    m_patient = patient;
    m_metodicName = metodicName;
    m_dt = QDateTime::currentDateTime();

    m_probes.clear();

    newProbe(""); //todo: название первой пробы
}

void TestResultData::newProbe(const QString &probeName)
{
    Q_ASSERT(m_mode == mdRun);

    ProbeResultData *probe = new ProbeResultData(m_uid, QUuid::createUuid().toString(), probeName);
    m_probes << probe;
}

void TestResultData::saveTest()
{
    Q_ASSERT(m_mode == mdRun);

    foreach (auto* probe, m_probes)
    {
        probe->saveProbe();
        delete probe;
    }
    m_probes.clear();

    m_mode = mdUndefined;
}

void TestResultData::openTest(const QString &uid)
{
    Q_ASSERT(m_mode == mdUndefined);
    m_mode = mdHandle;
    m_uid = uid;
}

void TestResultData::closeTest()
{
    foreach (auto* probe, m_probes)
    {
        probe->clear();
        delete probe;
    }
    m_probes.clear();
}

void TestResultData::addSignal(SignalData *signal)
{
    Q_ASSERT(m_mode == mdRun);
    m_probes.at(m_probes.size() - 1)->addSignal(signal);
}

int TestResultData::probesCount() const
{
    return m_probes.size();
}

ProbeResultInfo *TestResultData::probe(const int number) const
{
    Q_ASSERT(number >= 0 && number < m_probes.size());
    return new ProbeResultInfo(m_probes.at(number));
}
