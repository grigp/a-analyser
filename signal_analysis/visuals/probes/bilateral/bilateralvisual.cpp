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
    return QCoreApplication::tr("Билатеральный режим"); //VisualNames::Name_Bilateral;
}

Visual *BilateralVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelId,
                                         const QString& sectionNumber)
{
    Q_UNUSED(channelId);
    Q_UNUSED(sectionNumber);
    return new BilateralVisualWidget(this, testUid, probeUid, parent);
}
