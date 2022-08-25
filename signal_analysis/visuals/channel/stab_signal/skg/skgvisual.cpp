#include "skgvisual.h"

#include "skgvisualwidget.h"

SKGVisual::SKGVisual(BaseDefines::TestLevel level)
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

Visual *SKGVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelId,
                                   const QString& sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new SKGVisualWidget(this, testUid, probeUid, channelId, parent);
}
