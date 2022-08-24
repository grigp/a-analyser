#include "dynamosignalvisual.h"

#include "dynamosignalvisualwidget.h"

DynamoSignalVisual::DynamoSignalVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

DynamoSignalVisual::~DynamoSignalVisual()
{

}

QString DynamoSignalVisual::uid()
{
    return VisualNames::UID_DynamoSignal;
}

QString DynamoSignalVisual::name()
{
    return VisualNames::Name_DynamoSignal;
}

Visual *DynamoSignalVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return new DynamoSignalVisualWidget(this, testUid, probeUid, channelId, parent);
}
