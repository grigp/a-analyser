#include "mancoordppvisual.h"

#include "mancoordppvisualwidget.h"

ManCoordPPVisual::ManCoordPPVisual()
{

}

ManCoordPPVisual::~ManCoordPPVisual()
{

}

QString ManCoordPPVisual::uid()
{
    return PPVisualNames::UID_ManCoord;
}

QString ManCoordPPVisual::name()
{
    return PPVisualNames::Name_ManCoord;
}

PPVisual *ManCoordPPVisual::getVisualWidget(QJsonObject &objPP, QWidget *parent)
{
    return new ManCoordPPVisualWidget(this, objPP, parent);
}
