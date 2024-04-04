#include "babyposturecalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "classicfactors.h"

BabyPostureCalculator::BabyPostureCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

void BabyPostureCalculator::calculate()
{
    StabTestCalculator::calculate();

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        if (ti.probes.size() > 0)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(0), pi))
            {
                m_fctClassic = new ClassicFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                m_s = m_fctClassic->factorValueFormatted(ClassicFactorsDefines::SquareUid);
                m_ellLen = m_fctClassic->factorValueFormatted(ClassicFactorsDefines::EllLengthUid);
                m_lfs = m_fctClassic->factorValueFormatted(ClassicFactorsDefines::LFSUid);
                m_ellE = m_fctClassic->factorValueFormatted(ClassicFactorsDefines::ComprRatioUid);
            }
        }
    }
}

void BabyPostureCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();
}
