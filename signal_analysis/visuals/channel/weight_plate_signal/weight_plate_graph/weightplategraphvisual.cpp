#include "weightplategraphvisual.h"

#include "weightplategraphvisualwidget.h"

WeightPlateGraphVisual::WeightPlateGraphVisual(BaseDefines::TestLevel level)
    : VisualDescriptor (level)
{

}

WeightPlateGraphVisual::~WeightPlateGraphVisual()
{

}

QString WeightPlateGraphVisual::uid()
{
    return VisualNames::UID_WeightPlateGraph;
}

QString WeightPlateGraphVisual::name()
{
    return VisualNames::Name_WeightPlateGraph;
}

Visual *WeightPlateGraphVisual::getVisualWidget(QWidget *parent,
                                                const QString &testUid,
                                                const QString &probeUid,
                                                const QString &channelId,
                                                const QString& sectionNumber)
{
    Q_UNUSED(sectionNumber);
    return new WeightPlateGraphVisualWidget(this, testUid, probeUid, channelId, parent);
}
