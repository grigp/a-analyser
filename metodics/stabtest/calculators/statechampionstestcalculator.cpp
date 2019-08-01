#include "statechampionstestcalculator.h"

#include "classicfactors.h"
#include "vectorfactors.h"
#include "ratioprobesfactors.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"

#include <QDebug>

StateChampionsTestCalculator::StateChampionsTestCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

void StateChampionsTestCalculator::calculate()
{
    StabTestCalculator::calculate();

    //! Коэффициент Ромберга
    auto rf = new RatioProbesFactors(testUid());
    m_RombKoef = rf->factorValue(RatioProbesFactorsDefines::Probe2SUid);
    delete rf;

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
                auto cf = new ClassicFactors(testUid(), ti.probes.at(i), ChannelsDefines::chanStab);
                m_prbFct[i].square = cf->factorValue(ClassicFactorsDefines::SquareUid);
                addPrimaryFactor(ClassicFactorsDefines::SquareUid, m_prbFct[i].square,
                                 i, ChannelsDefines::chanStab, pi.name);
                delete cf;

                auto vf = new VectorFactors(testUid(), ti.probes.at(i), ChannelsDefines::chanStab);
                m_prbFct[i].speed = vf->factorValue(VectorFactorsDefines::VMidUid);
                addPrimaryFactor(VectorFactorsDefines::VMidUid, m_prbFct[i].speed,
                                 i, ChannelsDefines::chanStab, pi.name);
                if (i == 2)
                {
                    m_trgtKFR = vf->factorValue(VectorFactorsDefines::KFRUid);
                    addPrimaryFactor(VectorFactorsDefines::KFRUid, m_trgtKFR,
                                     i, ChannelsDefines::chanStab, pi.name);
                }
                delete vf;
            }
        }
    }

    addPrimaryFactor(RatioProbesFactorsDefines::Probe2SUid, m_RombKoef, tr("Коэффициент Ромберга"));
}

void StateChampionsTestCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();
}

double StateChampionsTestCalculator::speed(const int probeNum) const
{
    Q_ASSERT(probeNum >= 0 && probeNum < 3);
    return m_prbFct[probeNum].speed;
}

double StateChampionsTestCalculator::square(const int probeNum) const
{
    Q_ASSERT(probeNum >= 0 && probeNum < 3);
    return m_prbFct[probeNum].square;
}

double StateChampionsTestCalculator::rombKoef() const
{
    return m_RombKoef;
}

double StateChampionsTestCalculator::targetKFR() const
{
    return m_trgtKFR;
}
