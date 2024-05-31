#include "mwcompetitivestickcalculator.h"

#include "mwstickforcefactors.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"

MWCompetitiveStickCalculator::MWCompetitiveStickCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void MWCompetitiveStickCalculator::calculate()
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
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanMWStickForce))
                {
                    m_factors = new MWStickForceFactors(testUid(), pi.uid, ChannelsDefines::chanMWStickForce);

                    for (int i = 0; i < m_factors->size(); ++i)
                    {
                        addPrimaryFactor(testUid(), m_factors->factorUid(i),
                                         m_factors->factorValue(m_factors->factorUid(i)),
                                         0, ChannelsDefines::chanStab, pi.name);
                    }
                    break;
                }
            }
        }
    }
}

void MWCompetitiveStickCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

double MWCompetitiveStickCalculator::maxForce()
{
    if (m_factors)
        return m_factors->factorValue(MWStickForceFactorsDefines::MaxForceUid);
    return 0;
}

QString MWCompetitiveStickCalculator::maxForceFmt()
{
    if (m_factors)
        return m_factors->factorValueFormatted(MWStickForceFactorsDefines::MaxForceUid);
    return "";
}

double MWCompetitiveStickCalculator::averageForce()
{
    if (m_factors)
        return m_factors->factorValue(MWStickForceFactorsDefines::AverageForceUid);
    return 0;
}

QString MWCompetitiveStickCalculator::averageForceFmt()
{
    if (m_factors)
        return m_factors->factorValueFormatted(MWStickForceFactorsDefines::AverageForceUid);
    return "";
}
