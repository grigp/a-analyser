#include "stepdeviationcalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "stepdeviationfactors.h"
#include "metodicsfactory.h"
#include "aanalyserapplication.h"

#include <QDebug>

StepDeviationCalculator::StepDeviationCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void StepDeviationCalculator::calculate()
{
    TestCalculator::calculate();

    //! Необходимо в тесте, в котором будет возможно ни одна проба найти нужную пробу
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        //! uid шаблона методики
        MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
        auto mi = metFactory->metodic(ti.metodUid);

        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab) &&
                    (mi.templateId == MetodicDefines::MetUid_StepDeviatoin))
                {
                    m_factors = new StepDeviationFactors(testUid(), pi.uid);

                    for (int i = 0; i < m_factors->size(); ++i)
                    {
                        addPrimaryFactor(testUid(), m_factors->factorUid(i),
                                         m_factors->factorValue(m_factors->factorUid(i)),
                                         0, ChannelsDefines::chanStab, pi.name);
                    }
                }
            }
        }
    }
}

void StepDeviationCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

int StepDeviationCalculator::signalSize() const
{
    if (m_factors)
        return m_factors->signalSize();
    return 0;
}

int StepDeviationCalculator::signalFrequency() const
{
    if (m_factors)
        return m_factors->signalFreqency();
    return 0;
}

double StepDeviationCalculator::signal(const int idx) const
{
    if (m_factors)
        return m_factors->signal(idx);
    return 0;
}

double StepDeviationCalculator::signalFiltred(const int idx) const
{
    if (m_factors)
        return m_factors->signalFiltred(idx);
    return 0;
}

int StepDeviationCalculator::growthDynCount() const
{
    if (m_factors)
        return m_factors->growthDynCount();
    return 0;
}

double StepDeviationCalculator::growthDynValue(const int idx) const
{
    if (m_factors)
        return m_factors->growthDynValue(idx);
    return 0;
}

int StepDeviationCalculator::lengthDynCount() const
{
    if (m_factors)
        return m_factors->lengthDynCount();
    return 0;
}

double StepDeviationCalculator::lengthDynValue(const int idx) const
{
    if (m_factors)
        return m_factors->lengthDynValue(idx);
    return 0;
}

BaseDefines::Directions StepDeviationCalculator::direction() const
{
    if (m_factors)
        return m_factors->direction();
    return BaseDefines::dirUp;
}
