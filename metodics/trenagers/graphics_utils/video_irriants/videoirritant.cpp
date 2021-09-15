#include "videoirritant.h"

#include "linesirriant.h"

#include <QPainter>

VideoIrritant::VideoIrritant(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsItem (parent)
{
    m_rect = rect;
    setOpacity(0.5);

    m_irriants << new LinesIrriant();
}

VideoIrritant::~VideoIrritant()
{
    m_irriants.clear();
}

QRectF VideoIrritant::boundingRect() const
{
    return m_rect;
}

void VideoIrritant::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (m_current >= 0)
    {
        m_irriants.at(m_current)->paint(painter, option, widget, boundingRect());
    }
}

int VideoIrritant::irriantsCount() const
{
    return m_irriants.size();
}

Irriant *VideoIrritant::irriant(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_irriants.size());
    return m_irriants.at(idx);
}

void VideoIrritant::setCurrentIrriant(const int idx)
{
    Q_ASSERT(idx >= 0 && idx < m_irriants.size());
    m_current = idx;
}

Irriant::Irriant()
    : QObject()
{

}


Irriant::~Irriant()
{
    
}
