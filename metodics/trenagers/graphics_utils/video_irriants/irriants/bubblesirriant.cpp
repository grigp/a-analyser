#include "bubblesirriant.h"

#include "bubblesirriantsettingswidget.h"

#include <QPainter>
#include <QDebug>


BubblesIrriant::BubblesIrriant()
    : Irriant()
{
    initBubbles();
}

void BubblesIrriant::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF boundingRect)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

}

QWidget *BubblesIrriant::getSettingsWidget()
{
    return new BubblesIrriantSettingsWidget(this);
}

void BubblesIrriant::setBubblesCount(const int cnt)
{
    m_bubblesCount = cnt;
    initBubbles();
}

void BubblesIrriant::initBubbles()
{

}
