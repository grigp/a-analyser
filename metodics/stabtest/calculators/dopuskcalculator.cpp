#include "dopuskcalculator.h"

#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "vectorfactors.h"

#include <QDebug>

DopuskCalculator::DopuskCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

void DopuskCalculator::calculate()
{
    StabTestCalculator::calculate();

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            if (i > 2)
                break;
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                m_factors[i] = new VectorFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                addPrimaryFactor(VectorFactorsDefines::KFRUid,
                                 m_factors[i]->factorValue(VectorFactorsDefines::KFRUid),
                                 pi.name);
            }
        }
    }
}

void DopuskCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();
}

VectorFactors *DopuskCalculator::factors(const int probeNum) const
{
    Q_ASSERT(probeNum >= 0 && probeNum < 3);
    return m_factors[probeNum];
}
