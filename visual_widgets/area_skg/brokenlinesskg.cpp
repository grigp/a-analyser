#include "brokenlinesskg.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>

BrokenLinesSKG::BrokenLinesSKG(int diap, QGraphicsItem *parent)
    : QGraphicsItem (parent)
    , m_diap(diap)
{

}

QRectF BrokenLinesSKG::boundingRect() const
{
    QPointF pos(-m_diap * m_prop - AreaSKGDefines::I_LABEL_SPACE, -m_diap * m_prop - AreaSKGDefines::I_LABEL_SPACE);
    QSizeF size((m_diap * m_prop + AreaSKGDefines::I_LABEL_SPACE) * 2, (m_diap * m_prop + AreaSKGDefines::I_LABEL_SPACE) * 2);
    return QRectF(pos, size);
}

void BrokenLinesSKG::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(item);
    // Параметры построения
    m_width = widget->size().width();
    m_height = widget->size().height();
    int minS = qMin(m_width, m_height);
    m_prop = static_cast<double>(minS / 2 - AreaSKGDefines::I_LABEL_SPACE) / static_cast<double>(m_diap);

    painter->save();

    //! По ломаным, их может быть несколько
    foreach (auto bLine, m_brokenLines)
    {
        painter->setPen(QPen(bLine.color, bLine.width));

        //! По точкам ломаной
        QPolygonF plgn;
        foreach (auto point, bLine.polygon)
        {
            double x = point.x() * m_prop;
            double y = - point.y() * m_prop;
            plgn << QPointF(x, y);
        }
        double x = bLine.polygon.at(0).x() * m_prop;
        double y = - bLine.polygon.at(0).y() * m_prop;
        plgn << QPointF(x, y);

        painter->drawPolyline(plgn);
    }

    painter->restore();
}

int BrokenLinesSKG::addBrokenLine(AreaSKGDefines::BrokenLine &bl)
{
    m_brokenLines << bl;
    return  m_brokenLines.size() - 1;
}

bool BrokenLinesSKG::deleteBrokenLine(const int idx)
{
    if (idx >= 0 && idx < m_brokenLines.size())
    {
        m_brokenLines.removeAt(idx);
        return true;
    }
    return false;
}

void BrokenLinesSKG::setDiap(int diap)
{
    //! Если до первой прорисовки сделать изменение масштаба, появятся коллизии
//    if (m_width > 0 && m_height > 0)
//    {
        m_diap = diap;
//        int minS = qMin(m_width, m_height);
//        m_prop = static_cast<double>(minS / 2 - GridSKGDefines::I_LABEL_SPACE) / static_cast<double>(m_diap);
        update(boundingRect());
//    }
}
