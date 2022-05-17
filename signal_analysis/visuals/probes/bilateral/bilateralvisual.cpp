#include "bilateralvisual.h"
#include "bilateralvisualwidget.h"


BilateralVisual::BilateralVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

QString BilateralVisual::uid()
{
    return VisualNames::UID_Bilateral;
}

QString BilateralVisual::name()
{
    return VisualNames::Name_Bilateral;
}

Visual *BilateralVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(channelUid);
    return new BilateralVisualWidget(this, testUid, probeUid, parent);
}
