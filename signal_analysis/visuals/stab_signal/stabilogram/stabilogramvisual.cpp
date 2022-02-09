#include "stabilogramvisual.h"

#include "stabilogramvisualwidget.h"

StabilogramVisual::StabilogramVisual(const QString& testUid, const QString& probeUid, const QString& channelUid)
    : ChannelVisual (testUid, probeUid, channelUid)
{

}

QString StabilogramVisual::uid()
{

}

QString StabilogramVisual::name()
{

}

bool StabilogramVisual::isValid()
{

}

Visual *StabilogramVisual::getVisualWidget()
{
    return new StabilogramVisualWidget(this);

}
