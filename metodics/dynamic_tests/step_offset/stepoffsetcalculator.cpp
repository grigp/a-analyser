#include "stepoffsetcalculator.h"

#include <QUuid>
#include <QDebug>

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "stepoffsetfactors.h"

namespace
{
  QSet<QString> PrimaryFactors =
  {
        QString(StepOffsetFactorsDefines::Compensation::ReactionTimeUid)
      , QString(StepOffsetFactorsDefines::Compensation::ReactionTimeUid)
      , QString(StepOffsetFactorsDefines::Compensation::StatismUid)
      , QString(StepOffsetFactorsDefines::Compensation::LatentUid)
      , QString(StepOffsetFactorsDefines::Compensation::SpurtSpeedMMUid)
      , QString(StepOffsetFactorsDefines::Compensation::SpurtAmplUid)
      , QString(StepOffsetFactorsDefines::Compensation::StabilityDeviationUid)
      , QString(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid)
      , QString(StepOffsetFactorsDefines::Compensation::CorrectKognErrorUid)
      , QString(StepOffsetFactorsDefines::Compensation::CorrectMotorErrorUid)
      , QString(StepOffsetFactorsDefines::Compensation::ProcessKindUid)
      , QString(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid)
  };

}

StepOffsetCalculator::StepOffsetCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void StepOffsetCalculator::calculate()
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
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanStepOffsetResult))
                {
                    m_factors = new StepOffsetFactors(testUid(), pi.uid);

                    //! Добавление приоритетных показателей
                    foreach (auto uid, PrimaryFactors)
                        addPrimaryFactor(testUid(), uid, m_factors->factorValue(uid),
                                         0, ChannelsDefines::chanStab, pi.name);
                }
            }
        }
    }
}

void StepOffsetCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();

}

int StepOffsetCalculator::stageTime() const
{
    if (m_factors)
        return m_factors->stageTime();
    return 0;
}

int StepOffsetCalculator::freq() const
{
    if (m_factors)
        return m_factors->freq();
    return 0;
}

int StepOffsetCalculator::diap() const
{
    if (m_factors)
        return m_factors->diap();
    return 0;
}

int StepOffsetCalculator::force() const
{
    if (m_factors)
        return m_factors->force();
    return 0;
}

BaseUtils::Directions StepOffsetCalculator::direction() const
{
    if (m_factors)
        return m_factors->direction();
    return BaseUtils::dirNone;
}

int StepOffsetCalculator::stepsCount() const
{
    if (m_factors)
        return m_factors->stepsCount();
    return 0;
}

int StepOffsetCalculator::bufferCompensationCount() const
{
    if (m_factors)
        return m_factors->bufferCompensationCount();
    return 0;
}

double StepOffsetCalculator::bufferCompensationValue(const int i) const
{
    if (m_factors)
        return m_factors->bufferCompensationValue(i);
    return 0;
}

int StepOffsetCalculator::bufferReturnCount() const
{
    if (m_factors)
        return m_factors->bufferReturnCount();
    return 0;
}

double StepOffsetCalculator::bufferReturnValue(const int i) const
{
    if (m_factors)
        return m_factors->bufferReturnValue(i);
    return 0;
}

int StepOffsetCalculator::factorsCount() const
{
    if (m_factors)
        return m_factors->size();
    return 0;
}

QString StepOffsetCalculator::factorUid(const int id) const
{
    if (m_factors)
        return m_factors->factorUid(id);
    return QString(QUuid().toString());
}

double StepOffsetCalculator::factorValue(const int id) const
{
    if (m_factors)
        return m_factors->factorValue(id);
    return 0;
}

double StepOffsetCalculator::factorValue(const QString &uid) const
{
    if (m_factors)
        return m_factors->factorValue(uid);
    return 0;
}

QString StepOffsetCalculator::factorValueFormatted(const QString &uid) const
{
    if (m_factors)
        return m_factors->factorValueFormatted(uid);
    return QString("0");
}
