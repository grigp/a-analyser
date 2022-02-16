#include "visualsfactory.h"

#include <QTimer>

#include "visualdescriptor.h"
#include "aanalyserbuild.h"

VisualsFactory::VisualsFactory(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(0, [=]()
    {
        AAnalyserBuild::registerVisuals();
    });
}

VisualsFactory::~VisualsFactory()
{
    foreach (auto vis, m_visTest)
        delete vis;
    m_visTest.clear();
    foreach (auto vis, m_visProbe)
        delete vis;
    m_visProbe.clear();
    foreach (auto vis, m_visChannel)
        delete vis;
    m_visChannel.clear();
}

void VisualsFactory::registerVisual(VisualDescriptor *visual)
{
    if (visual->level() == VisualDefines::vlTest)
        m_visTest << visual;
    else
    if (visual->level() == VisualDefines::vlProbe)
        m_visProbe << visual;
    else
    if (visual->level() == VisualDefines::vlChannel)
        m_visChannel << visual;
}

int VisualsFactory::visualCount(const VisualDefines::Level level)
{
    switch (level) {
    case VisualDefines::vlTest: return m_visTest.size();
    case VisualDefines::vlProbe: return m_visProbe.size();
    case VisualDefines::vlChannel: return m_visChannel.size();
    case VisualDefines::vlNone: return 0;
    }
    return 0;
}

VisualDescriptor *VisualsFactory::getVisual(const VisualDefines::Level level, const int idx)
{
    switch (level) {
    case VisualDefines::vlTest:
    {
        Q_ASSERT(idx >= 0 && idx < m_visTest.size());
        return m_visTest.at(idx);
    }
    case VisualDefines::vlProbe:
    {
        Q_ASSERT(idx >= 0 && idx < m_visProbe.size());
        return m_visProbe.at(idx);
    }
    case VisualDefines::vlChannel:
    {
        Q_ASSERT(idx >= 0 && idx < m_visChannel.size());
        return m_visChannel.at(idx);
    }
    case VisualDefines::vlNone: return nullptr;
    }
    return nullptr;
}

