#include "ritmogramvisual.h"

#include "ritmogramvisualwidget.h"

RitmogramVisual::RitmogramVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

QString RitmogramVisual::uid()
{
    return VisualNames::UID_Ritmogram;
}

QString RitmogramVisual::name()
{
    return VisualNames::Name_Ritmogram;
}

Visual *RitmogramVisual::getVisualWidget(QWidget *parent,
                                         const QString &testUid,
                                         const QString &probeUid,
                                         const QString &channelId,
                                         const QString &sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new RitmogramVisualWidget(this, testUid, probeUid, channelId, parent);
}
