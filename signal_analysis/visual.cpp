#include "visual.h"

Visual::Visual(BaseVisual* visual, QWidget *parent) :
    QWidget(parent)
  , m_visual(visual)
{
}

Visual::~Visual()
{
}
