#include "traceskg.h"
#include <QPainter>
#include <QWidget>

static const int I_LABEL_SPACE = 20;

TraceSKG::TraceSKG(int diap, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_diap(diap)
{
    m_trace.clear();
}

QRectF TraceSKG::boundingRect() const
{
    QPointF pos(-m_diap * m_prop - I_LABEL_SPACE, -m_diap * m_prop - I_LABEL_SPACE);
    QSizeF size((m_diap * m_prop + I_LABEL_SPACE) * 2, (m_diap * m_prop + I_LABEL_SPACE) * 2);
    return QRectF(pos, size);
}

void TraceSKG::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    // Параметры построения
    m_width = widget->size().width();
    m_height = widget->size().height();
    int minS = qMin(m_width, m_height);
    m_prop = static_cast<double>(minS / 2 - I_LABEL_SPACE) / static_cast<double>(m_diap);

    painter->save();

    painter->setPen(QPen(Qt::blue, 1));

    int i = 0;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    QListIterator<QPointF> it(m_trace);
    while (it.hasNext())
    {
        QPointF pt = it.next();
        x1 = pt.x() * m_prop;
        y1 = - pt.y() * m_prop;

        if (i > 0)
            painter->drawLine(x1, y1, x2, y2);
        x2 = x1;
        y2 = y1;

        ++i;
    }

    painter->restore();
}

void TraceSKG::setDiap(int diap)
{
    m_diap = diap;
    updateItem();
}

void TraceSKG::add(double x, double y)
{
    QPointF pt(x, y);
    m_trace.append(pt);
    updateItem();
}

void TraceSKG::clear()
{
    m_trace.clear();
    updateItem();
}

void TraceSKG::updateItem()
{
    int minS = qMin(m_width, m_height);
    m_prop = static_cast<double>(minS / 2 - I_LABEL_SPACE) / static_cast<double>(m_diap);
    update(boundingRect());
}
