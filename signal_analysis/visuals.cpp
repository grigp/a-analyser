#include "visuals.h"

Visual::Visual(VisualDescriptor* visual, QWidget *parent) :
    QWidget(parent)
  , m_visual(visual)
{
}

Visual::~Visual()
{
}

TestVisual::TestVisual(VisualDescriptor *visual, const QString &testUid, QWidget *parent)
    : Visual (visual, parent)
    , m_testUid(testUid)
{

}

ProbeVisual::ProbeVisual(VisualDescriptor *visual,
                         const QString &testUid, const QString &probeUid,
                         QWidget *parent)
    : TestVisual (visual, testUid, parent)
    , m_probeUid(probeUid)
{

}

ChannelVisual::ChannelVisual(VisualDescriptor *visual,
                             const QString &testUid, const QString &probeUid, const QString &channelUid,
                             QWidget *parent)
    : ProbeVisual (visual, testUid, probeUid, parent)
    , m_channelUid(channelUid)
{

}
