#include "spectrstabvisual.h"

#include "spectrstabvisualwidget.h"

SpectrStabVisual::SpectrStabVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

SpectrStabVisual::~SpectrStabVisual()
{

}

QString SpectrStabVisual::uid()
{
    return VisualNames::UID_SpectrStab;
}

QString SpectrStabVisual::name()
{
    return VisualNames::Name_SpectrStab;
}

Visual *SpectrStabVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return new SpectrStabVisualWidget(this, testUid, probeUid, channelId, parent);
}
