#include "bedsidescalestestcalculator.h"

#include <QDebug>

#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "weightplatesignal.h"
#include "balistogram.h"
#include "weighingresultdata.h"

BedsideScalesTestCalculator::BedsideScalesTestCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

BedsideScalesTestCalculator::~BedsideScalesTestCalculator()
{
    if (m_signal)
        delete m_signal;
}

void BedsideScalesTestCalculator::calculate()
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
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanWeight) ||
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanWeightPlate))
                    getSignal(pi.uid);

                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanParticalWeighting))
                {
                    QByteArray data;
                    if (DataProvider::getChannel(pi.uid, ChannelsDefines::chanParticalWeighting, data))
                        m_wd = new WeighingResultData(data);
                }
            }
        }
    }
}

void BedsideScalesTestCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

int BedsideScalesTestCalculator::signalFrequency() const
{
    if (m_signal)
        return m_signal->frequency();
    return 150;
}

int BedsideScalesTestCalculator::signalSize() const
{
    if (m_signal)
        return m_signal->size();
    return 0;
}

int BedsideScalesTestCalculator::subChansCount() const
{
    if (m_signal)
        return m_signal->subChansCount();
    return 0;
}

double BedsideScalesTestCalculator::signalValue(const int subChan, const int rec) const
{
    if (m_signal)
        return m_signal->value(subChan, rec);
    return 0;
}

bool BedsideScalesTestCalculator::isParticalWeighting() const
{
    return (m_wd);
//    return (m_wd != nullptr);
}

QTime BedsideScalesTestCalculator::scalingInterval() const
{
    if (m_wd)
        return m_wd->scalingInterval();
    return QTime(0, 0);
}

int BedsideScalesTestCalculator::averageTime() const
{
    if (m_wd)
        return m_wd->averageTime();
    return 0;
}

int BedsideScalesTestCalculator::size() const
{
    if (m_wd)
        return m_wd->size();
    return 0;
}

double BedsideScalesTestCalculator::weight(const int idx) const
{
    if (m_wd)
        return m_wd->weight(idx);
    return 0;
}

void BedsideScalesTestCalculator::getSignal(const QString &probeId)
{
    QByteArray data;
    if (!m_signal)
    {
        if (DataProvider::channelExists(probeId, ChannelsDefines::chanWeightPlate))
        {
            if (DataProvider::getChannel(probeId, ChannelsDefines::chanWeightPlate, data))
                m_signal = new WeightPlateSignal(data);
        }
        if (DataProvider::channelExists(probeId, ChannelsDefines::chanWeight))
        {
            if (DataProvider::getChannel(probeId, ChannelsDefines::chanWeight, data))
                m_signal = new Balistogram(data);
        }
    }
}
