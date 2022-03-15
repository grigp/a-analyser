#include "weightplategraphvisual.h"

#include "weightplategraphvisualwidget.h"

WeightPlateGraphVisual::WeightPlateGraphVisual(VisualDefines::Level level)
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

Visual *WeightPlateGraphVisual::getVisualWidget(QWidget *parent, const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new WeightPlateGraphVisualWidget(this, testUid, probeUid, channelUid, parent);
}
