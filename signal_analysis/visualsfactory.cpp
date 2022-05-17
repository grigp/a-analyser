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
    if (visual->level() == BaseDefines::tlTest)
        m_visTest << visual;
    else
    if (visual->level() == BaseDefines::tlProbe)
        m_visProbe << visual;
    else
    if (visual->level() == BaseDefines::tlChannel)
        m_visChannel << visual;
}

int VisualsFactory::visualCount(const BaseDefines::TestLevel level)
{
    switch (level) {
    case BaseDefines::tlTest: return m_visTest.size();
    case BaseDefines::tlProbe: return m_visProbe.size();
    case BaseDefines::tlChannel: return m_visChannel.size();
    case BaseDefines::tlNone: return 0;
    }
    return 0;
}

VisualDescriptor *VisualsFactory::getVisual(const BaseDefines::TestLevel level, const int idx)
{
    switch (level) {
    case BaseDefines::tlTest:
    {
        Q_ASSERT(idx >= 0 && idx < m_visTest.size());
        return m_visTest.at(idx);
    }
    case BaseDefines::tlProbe:
    {
        Q_ASSERT(idx >= 0 && idx < m_visProbe.size());
        return m_visProbe.at(idx);
    }
    case BaseDefines::tlChannel:
    {
        Q_ASSERT(idx >= 0 && idx < m_visChannel.size());
        return m_visChannel.at(idx);
    }
    case BaseDefines::tlNone: return nullptr;
    }
    return nullptr;
}

