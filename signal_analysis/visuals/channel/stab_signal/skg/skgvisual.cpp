#include "skgvisual.h"

#include "skgvisualwidget.h"

SKGVisual::SKGVisual(VisualDefines::Level level)
    : VisualDescriptor (level)
{

}

QString SKGVisual::uid()
{
    return VisualNames::UID_SKG;
}

QString SKGVisual::name()
{
    return VisualNames::Name_SKG;
}

Visual *SKGVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new SKGVisualWidget(this, testUid, probeUid, channelUid, parent);
}
