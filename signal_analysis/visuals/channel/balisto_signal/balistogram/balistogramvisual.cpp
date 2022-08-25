#include "balistogramvisual.h"


#include "balistogramvisualwidget.h"

BalistogramVisual::BalistogramVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

BalistogramVisual::~BalistogramVisual()
{

}

QString BalistogramVisual::uid()
{
    return VisualNames::UID_Balistogram;
}

QString BalistogramVisual::name()
{
    return VisualNames::Name_Balistogram;
}

Visual *BalistogramVisual::getVisualWidget(QWidget *parent,
                                           const QString &testUid, const QString &probeUid, const QString &channelId, const QString& sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new BalistogramVisualWidget(this, testUid, probeUid, channelId, parent);
}
