#include "testresultdata.h"

#include "proberesultdata.h"
#include "resultinfo.h"
#include "dataprovider.h"

#include <QUuid>
#include <QDebug>

TestResultData::TestResultData()
{
    m_mode = mdUndefined;
}

void TestResultData::newTest(const QString &patientUid, const QString &metodicUid)
{
    Q_ASSERT(m_mode == mdUndefined);
    m_mode = mdRun;

    m_uid = QUuid::createUuid().toString();
    m_patientUid = patientUid;
    m_metodicUid = metodicUid;
    m_dt = QDateTime::currentDateTime();

    m_probes.clear();
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

    auto testUid = DataProvider::addTest(m_patientUid, m_metodicUid);

    int n = 0;
    foreach (auto* probe, m_probes)
    {
        probe->saveProbe(testUid, n);
        delete probe;
        ++n;
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
