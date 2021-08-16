#include "trentakeputtestcalculator.h"

#include "channelsdefines.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "trenresultfactors.h"

#include <QDebug>

TrenTakePutTestCalculator::TrenTakePutTestCalculator(const QString &testUid, QObject *parent)
    : TestCalculator(testUid, parent)
{

}

void TrenTakePutTestCalculator::calculate()
{
    TestCalculator::calculate();

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto factors = new TrenResultFactors(testUid(), pi.uid, ChannelsDefines::chanTrenResult);

                for (int j = 0; j < factors->size(); ++j)
                {
                    addPrimaryFactor(testUid(), factors->factorUid(j), factors->factorValue(j),
                                     i, ChannelsDefines::chanTrenResult, pi.name);

                    FactorsDefines::FactorValue fv(factors->factorUid(j), factors->factorValue(j));
                    m_factors << fv;
                }

                return;  //! Только для одной пробы
            }
        }
    }
}

void TrenTakePutTestCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();

}

FactorsDefines::FactorValue TrenTakePutTestCalculator::factor(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_factors.size());
    return m_factors.at(idx);
}
