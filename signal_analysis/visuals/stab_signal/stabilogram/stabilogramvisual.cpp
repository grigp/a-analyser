#include "stabilogramvisual.h"

#include "stabilogramvisualwidget.h"

StabilogramVisual::StabilogramVisual(VisualDefines::Level level)
    : VisualDescriptor (level)
{

}

QString StabilogramVisual::uid()
{
    return VisualNames::UID_Stabilogram;
}

QString StabilogramVisual::name()
{
    return VisualNames::Name_Stabilogram;
}

Visual *StabilogramVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new StabilogramVisualWidget(this, testUid, probeUid, channelUid, parent);
}

