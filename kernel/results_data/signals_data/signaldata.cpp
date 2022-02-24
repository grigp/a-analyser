#include "signaldata.h"

SignalData::SignalData()
    : ChannelData()
{

}

SignalData::~SignalData()
{

}

double SignalData::maxValue() const
{
    return m_maxValue;
}

double SignalData::minValue() const
{
    return m_minValue;
}

double SignalData::absMaxValue() const
{
    if (fabs(m_maxValue) > fabs(m_minValue))
        return fabs(m_maxValue);
    return fabs(m_minValue);
}

double SignalData::midValue() const
{
    return m_midValue;
}
