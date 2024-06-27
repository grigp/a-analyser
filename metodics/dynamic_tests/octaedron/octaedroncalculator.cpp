#include "octaedroncalculator.h"

#include <QDebug>

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "octaedronfactors.h"
#include "stabreacttrainfactors.h"

OctaedronCalculator::OctaedronCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void OctaedronCalculator::calculate()
{
    TestCalculator::calculate();

    //! Необходимо в тесте, в котором будет возможно ни одна проба найти нужную пробу
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            auto ssfm = ti.params["stage_fixing_mode"].toString();
            m_sfm = BaseDefines::StageFinishModeValueIndex.value(ssfm);

            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab) &&
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanOctaedronResult))
                {
                    if (m_sfm == BaseDefines::sfmFixedTime)
                    {
                        m_fctOctaedr = new OctaedronFactors(testUid(), pi.uid);

                        for (int i = 0; i < m_fctOctaedr->size(); ++i)
                            addPrimaryFactor(testUid(), m_fctOctaedr->factorUid(i),
                                             m_fctOctaedr->factorValue(m_fctOctaedr->factorUid(i)),
                                             0, ChannelsDefines::chanStab, pi.name);
                    }
                    else
                    if (m_sfm == BaseDefines::sfmFixingOnTarget)
                    {
                        m_fctSRT = new StabReactTrainFactors(testUid(), pi.uid);
                        for (int i = 0; i < m_fctSRT->size(); ++i)
                            addPrimaryFactor(testUid(), m_fctSRT->factorUid(i),
                                             m_fctSRT->factorValue(m_fctSRT->factorUid(i)),
                                             0, ChannelsDefines::chanStab, pi.name);
                    }

                }
            }
        }
    }
}

void OctaedronCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

double OctaedronCalculator::getValue(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < 8);
    if (m_fctOctaedr)
        return m_fctOctaedr->factorValue(idx + 1);
    return 0;
}

double OctaedronCalculator::getAverageValue() const
{
    if (m_fctOctaedr)
        return m_fctOctaedr->factorValue(FactorsDefines::CommonFactors::SuccessUid);
    return 0;
}

double OctaedronCalculator::latent(const int idx) const
{
    if (m_fctSRT)
        return m_fctSRT->latent(idx);
    return 0;
}

double OctaedronCalculator::time(const int idx) const
{
    if (m_fctSRT)
        return m_fctSRT->time(idx);
    return 0;
}

double OctaedronCalculator::latentAvg() const
{
    if (m_fctSRT)
        return m_fctSRT->latentAvg();
    return 0;
}

double OctaedronCalculator::timeAvg() const
{
    if (m_fctSRT)
        return m_fctSRT->timeAvg();
    return 0;
}

int OctaedronCalculator::stageTime() const
{
    if (m_fctOctaedr)
        return m_fctOctaedr->stageTime();
    return 5;
}

QString OctaedronCalculator::circeRoundRuleMode() const
{
    if (m_fctOctaedr)
        return m_fctOctaedr->circeRoundRuleMode();
    return BaseDefines::CirceRoundRuleModeValueName.value(BaseDefines::crmRadial);
}

QString OctaedronCalculator::directionMode() const
{
    if (m_fctOctaedr)
        return m_fctOctaedr->directionMode();
    return BaseDefines::DirectionModeValueName.value(BaseDefines::dmCounterClockwise);
}

int OctaedronCalculator::radius() const
{
    if (m_fctOctaedr)
        return m_fctOctaedr->radius();
    return 50;
}

int OctaedronCalculator::freq() const
{
    if (m_fctOctaedr)
        return m_fctOctaedr->freq();
    return 50;
}

int OctaedronCalculator::diap() const
{
    if (m_fctOctaedr)
        return m_fctOctaedr->diap();
    return 128;
}
