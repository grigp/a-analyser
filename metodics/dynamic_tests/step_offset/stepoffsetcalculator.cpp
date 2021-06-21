#include "stepoffsetcalculator.h"

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

                    int min = qMin(m_factors->bufferCompensationCount(), m_factors->bufferReturnCount());
                    for (int i = 0; i < min; ++i)
                    {
                        qDebug() << m_factors->bufferCompensationValue(i).x << m_factors->bufferCompensationValue(i).y << "    " <<
                                    m_factors->bufferReturnValue(i).x << m_factors->bufferReturnValue(i).y;
                    }

                }
            }
        }
    }
}

void StepOffsetCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();

}
