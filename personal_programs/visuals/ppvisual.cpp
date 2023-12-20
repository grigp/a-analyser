#include "ppvisual.h"

#include "ppvisualdescriptor.h"

PPVisual::PPVisual(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent) :
    QWidget(parent)
  , m_visual(visual)
  , m_objPP(objPP)
{
}

PPVisual::~PPVisual()
{
}

QString PPVisual::name()
{
    if (m_visual)
        return m_visual->name();
    return "";
}
