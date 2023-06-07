#include "spectrpulsevisual.h"

#include "spectrpulsevisualwidget.h"

SpectrPulseVisual::SpectrPulseVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

QString SpectrPulseVisual::uid()
{
    return VisualNames::UID_SpectrPulse;
}

QString SpectrPulseVisual::name()
{
    return VisualNames::Name_SpectrPulse;
}

Visual *SpectrPulseVisual::getVisualWidget(QWidget *parent,
                                           const QString &testUid,
                                           const QString &probeUid,
                                           const QString &channelId,
                                           const QString &sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new SpectrPulseVisualWidget(this, testUid, probeUid, channelId, parent);
}
