#include "variationpulsogramvisual.h"

#include "variationpulsogramvisualwidget.h"

VariationPulsogramVisual::VariationPulsogramVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

QString VariationPulsogramVisual::uid()
{
    return VisualNames::UID_VPG;
}

QString VariationPulsogramVisual::name()
{
    return QCoreApplication::tr("Вариационная пульсограмма"); //VisualNames::Name_VPG;
}

Visual *VariationPulsogramVisual::getVisualWidget(QWidget *parent,
                                                  const QString &testUid,
                                                  const QString &probeUid,
                                                  const QString &channelId,
                                                  const QString &sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new VariationPulsogramVisualWidget(this, testUid, probeUid, channelId, parent);
}
