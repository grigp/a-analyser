#include "testcalculator.h"

#include "datadefines.h"
#include "dataprovider.h"

TestCalculator::TestCalculator(const QString &testUid, QObject *parent)
    : QObject(parent)
    , m_testUid(testUid)
{

}

void TestCalculator::calculate()
{
    computeTestTree();
}

void TestCalculator::fastCalculate()
{

}

FactorsDefines::FactorValueDescript *TestCalculator::primaryFactor(const int num) const
{
    Q_ASSERT(num >= 0 && num < m_primaryFactors.size());
    return m_primaryFactors.at(num);
}

void TestCalculator::computeTestTree()
{
    m_mdlTest.clear();
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        auto *testItem = new QStandardItem(ti.uid);
        testItem->setData(ti.uid, TestUidRole);
        testItem->setData(ti.dateTime, TestDateTimeRole);
        testItem->setData(ti.patientUid, PatientUidRole);
        testItem->setData(ti.metodUid, MetodicUidRole);
        testItem->setData(ti.params, ParamsRole);

        m_mdlTest.appendRow(testItem);

        foreach (auto probeUid, ti.probes)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(probeUid, pi))
            {
                auto *probeItem = new QStandardItem(pi.uid);
            }
        }
    }

}

void TestCalculator::addPrimaryFactor(const QString &uid, const double value, const QString &description)
{
    m_primaryFactors.append(new FactorsDefines::FactorValueDescript(uid, value, description));
}
