#include "ppvisual.h"

#include "ppvisualdescriptor.h"

PPVisual::PPVisual(PPVisualDescriptor* visual, QWidget *parent) :
    QWidget(parent)
  , m_visual(visual)

{
}

PPVisual::~PPVisual()
{
}
