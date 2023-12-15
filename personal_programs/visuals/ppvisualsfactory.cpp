#include "ppvisualsfactory.h"

#include <QTimer>

#include "aanalyserbuild.h"

PPVisualsFactory::PPVisualsFactory(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(0, [=]()
    {
        AAnalyserBuild::registerPPVisuals();
    });

}

PPVisualsFactory::~PPVisualsFactory()
{

}

void PPVisualsFactory::registerVisual(PPVisualDescriptor *visual)
{
    m_visuals << visual;
}

int PPVisualsFactory::visualCount()
{
    return m_visuals.size();
}

PPVisualDescriptor *PPVisualsFactory::getVisual(const int idx)
{
    Q_ASSERT(idx >= 0 && idx < m_visuals.size());
    return m_visuals.at(idx);
}
