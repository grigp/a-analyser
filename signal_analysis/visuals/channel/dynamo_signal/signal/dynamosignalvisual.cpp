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
    return QCoreApplication::tr("Сигнал"); //VisualNames::Name_DynamoSignal;
}

Visual *DynamoSignalVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelId,
                                            const QString& sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new DynamoSignalVisualWidget(this, testUid, probeUid, channelId, parent);
}
