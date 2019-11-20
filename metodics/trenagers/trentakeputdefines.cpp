#include "trentakeputdefines.h"

QRectF TrenTakePutDefines::GameElement::boundingRect() const
{
    return QRectF(-25, -25, 50, 50);
}

void TrenTakePutDefines::GameElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
