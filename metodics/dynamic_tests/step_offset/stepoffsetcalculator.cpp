#include "stepoffsetcalculator.h"

#include <QUuid>
#include <QDebug>

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "stepoffsetfactors.h"

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

//                    int min = qMin(m_factors->bufferCompensationCount(), m_factors->bufferReturnCount());
//                    for (int i = 0; i < min; ++i)
//                    {
//                        qDebug() << m_factors->bufferCompensationValue(i).x << m_factors->bufferCompensationValue(i).y << "    " <<
//                                    m_factors->bufferReturnValue(i).x << m_factors->bufferReturnValue(i).y;
//                    }

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
