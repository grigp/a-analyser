#include "myogramsignalvisual.h"

#include "myogramsignalvisualwidget.h"

MyogramSignalVisual::MyogramSignalVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

MyogramSignalVisual::~MyogramSignalVisual()
{

}

QString MyogramSignalVisual::uid()
{
    return VisualNames::UID_MyogramSignal;
}

QString MyogramSignalVisual::name()
{
    return VisualNames::Name_MyogramSignal;
}

Visual *MyogramSignalVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new MyogramSignalVisualWidget(this, testUid, probeUid, channelUid, parent);
}
