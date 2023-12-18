#include "commonppvisual.h"

#include "commonppvisualwidget.h"

CommonPPVisual::CommonPPVisual()
    : PPVisualDescriptor ()
{

}

CommonPPVisual::~CommonPPVisual()
{

}

QString CommonPPVisual::uid()
{
    return PPVisualNames::UID_Common;
}

QString CommonPPVisual::name()
{
    return PPVisualNames::Name_Common;
}

PPVisual *CommonPPVisual::getVisualWidget(QJsonObject &objPP, QWidget *parent)
{
    return new CommonPPVisualWidget(this, objPP, parent);
}
