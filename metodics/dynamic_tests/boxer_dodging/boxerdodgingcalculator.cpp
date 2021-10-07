#include "boxerdodgingcalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "boxerdodgingmultifactor.h"

namespace
{
  QList<QString> PrimaryFactors =
  {
        QString(BoxerDodgingFactorsDefines::AverageLatUid)
      , QString(BoxerDodgingFactorsDefines::AverageTimeUid)
      , QString(BoxerDodgingFactorsDefines::AverageAmplUid)
      , QString(BoxerDodgingFactorsDefines::AllErrorsUid)
  };

}

BoxerDodgingCalculator::BoxerDodgingCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void BoxerDodgingCalculator::calculate()
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
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanBoxerDodgingResult))
                {
                    m_factors = new BoxerDodgingMultifactor(testUid(), pi.uid);

                    //! Добавление первичных показателей
                    foreach (auto uid, PrimaryFactors)
                        addPrimaryFactor(testUid(), uid, m_factors->factorValue(uid),
                                         0, ChannelsDefines::chanStab, pi.name);
                }
            }
        }
    }
}

void BoxerDodgingCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

double BoxerDodgingCalculator::factorValue(const QString uid) const
{
    if (m_factors)
        return  m_factors->factorValue(uid);
    return  0;
}

QString BoxerDodgingCalculator::factorValueFormatted(const QString &uid) const
{
    if (m_factors)
        return  m_factors->factorValueFormatted(uid);
    return "";
}

int BoxerDodgingCalculator::deviationThreshold() const
{
    if (m_factors)
        return  m_factors->deviationThreshold();
    return 0;
}

int BoxerDodgingCalculator::freq() const
{
    if (m_factors)
        return  m_factors->freq();
    return 50;
}

int BoxerDodgingCalculator::diap() const
{
    if (m_factors)
        return  m_factors->diap();
    return 0;
}

int BoxerDodgingCalculator::tryesCount() const
{
    if (m_factors)
        return  m_factors->tryesCount();
    return 0;
}

int BoxerDodgingCalculator::leftCount() const
{
    if (m_factors)
        return  m_factors->leftCount();
    return 0;
}

int BoxerDodgingCalculator::rightCount() const
{
    if (m_factors)
        return  m_factors->rightCount();
    return 0;
}

int BoxerDodgingCalculator::aheadCount() const
{
    if (m_factors)
        return  m_factors->aheadCount();
    return 0;
}

int BoxerDodgingCalculator::backCount() const
{
    if (m_factors)
        return  m_factors->backCount();
    return 0;
}

int BoxerDodgingCalculator::count(const BoxerDodgingDefines::Stages stage) const
{
    switch (stage)
    {
    case BoxerDodgingDefines::bdsLeftDodging : return leftCount();
    case BoxerDodgingDefines::bdsRightDodging : return rightCount();
    case BoxerDodgingDefines::bdsAheadBend : return aheadCount();
    case BoxerDodgingDefines::bdsBackBend : return backCount();
    case BoxerDodgingDefines::bdsBase : return tryesCount();
    }
    return 0;
}
