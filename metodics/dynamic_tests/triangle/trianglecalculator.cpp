#include "trianglecalculator.h"

#include "trianglefactors.h"
#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"

TriangleCalculator::TriangleCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void TriangleCalculator::calculate()
{
    TestCalculator::calculate();

    //! Необходимо в тесте, в котором будет возможно ни одна проба найти нужную пробу
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab) &&
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanTriangleResult))
                {
                    m_factors = new TriangleFactors(testUid(), pi.uid);

//                    for (int i = 0; i < m_factors->size(); ++i)
//                    {
//                        addPrimaryFactor(testUid(), m_factors->factorUid(i),
//                                         m_factors->factorValue(m_factors->factorUid(i)),
//                                         0, ChannelsDefines::chanStab, pi.name);
//                    }
                }
            }
        }
    }
}

void TriangleCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}
