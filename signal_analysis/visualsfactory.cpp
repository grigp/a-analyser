#include "visualsfactory.h"

#include "visuals.h"

VisualsFactory::VisualsFactory(QObject *parent) : QObject(parent)
{

}

void VisualsFactory::registerVisual(BaseVisual *visual)
{
    if (dynamic_cast<ChannelVisual*>(visual))
        m_visChannel << static_cast<ChannelVisual*>(visual);
    else
    if (dynamic_cast<ProbeVisual*>(visual))
        m_visProbe << static_cast<ProbeVisual*>(visual);
    else
    if (dynamic_cast<TestVisual*>(visual))
        m_visTest << static_cast<TestVisual*>(visual);
}

int VisualsFactory::testVisualsCount()
{
    return m_visTest.size();
}

TestVisual *VisualsFactory::getTestVisual(const int idx)
{
    Q_ASSERT(idx >= 0 && idx < m_visTest.size());
    return m_visTest.at(idx);
}

int VisualsFactory::probeVisualsCount()
{
    return m_visProbe.size();
}

TestVisual *VisualsFactory::getProbeVisual(const int idx)
{
    Q_ASSERT(idx >= 0 && idx < m_visProbe.size());
    return m_visProbe.at(idx);
}

int VisualsFactory::channelVisualsCount()
{
    return m_visChannel.size();
}

TestVisual *VisualsFactory::getChannelVisual(const int idx)
{
    Q_ASSERT(idx >= 0 && idx < m_visProbe.size());
    return m_visProbe.at(idx);
}
