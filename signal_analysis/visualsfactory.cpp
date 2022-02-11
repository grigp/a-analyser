#include "visualsfactory.h"

#include "visualdescriptor.h"

VisualsFactory::VisualsFactory(QObject *parent) : QObject(parent)
{

}

void VisualsFactory::registerVisual(VisualDescriptor *visual)
{
    if (visual->level() == VisualDescriptor::vlTest)
        m_visChannel << visual;
    else
    if (visual->level() == VisualDescriptor::vlProbe)
        m_visProbe << visual;
    else
    if (visual->level() == VisualDescriptor::vlChannel)
        m_visTest << visual;
}

int VisualsFactory::testVisualsCount()
{
    return m_visTest.size();
}

VisualDescriptor *VisualsFactory::getTestVisual(const int idx)
{
    Q_ASSERT(idx >= 0 && idx < m_visTest.size());
    return m_visTest.at(idx);
}

int VisualsFactory::probeVisualsCount()
{
    return m_visProbe.size();
}

VisualDescriptor *VisualsFactory::getProbeVisual(const int idx)
{
    Q_ASSERT(idx >= 0 && idx < m_visProbe.size());
    return m_visProbe.at(idx);
}

int VisualsFactory::channelVisualsCount()
{
    return m_visChannel.size();
}

VisualDescriptor *VisualsFactory::getChannelVisual(const int idx)
{
    Q_ASSERT(idx >= 0 && idx < m_visProbe.size());
    return m_visProbe.at(idx);
}
