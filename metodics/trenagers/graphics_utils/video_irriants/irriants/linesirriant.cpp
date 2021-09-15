#include "linesirriant.h"

#include "linesirriantsettingswidget.h"

#include <QPainter>

LinesIrriant::LinesIrriant()
    : Irriant ()
{

}

void LinesIrriant::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF boundingRect)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRect rect(static_cast<int>(boundingRect.x()), static_cast<int>(boundingRect.y()),
               static_cast<int>(boundingRect.width() / 2), static_cast<int>(boundingRect.height() / 2));

    painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));
    painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    painter->drawRect(rect);
}

QWidget* LinesIrriant::getSettingsWidget()
{
    return new LinesIrriantSettingsWidget();
}

