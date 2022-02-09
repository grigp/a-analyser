#include "visuals.h"

#include "visual.h"

BaseVisual::BaseVisual()
{

}

BaseVisual::~BaseVisual()
{

}


TestVisual::TestVisual(const QString &testUid)
    : BaseVisual ()
    , m_testUid(testUid)
{

}

ProbeVisual::ProbeVisual(const QString &testUid, const QString &probeUid)
    : TestVisual (testUid)
    , m_probeUid(probeUid)
{

}

ChannelVisual::ChannelVisual(const QString &testUid, const QString &probeUid, const QString &channelUid)
    : ProbeVisual (testUid, probeUid)
    , m_channelUid(channelUid)
{

}
