#include "tetrisglass.h"

#include <QPainter>

TetrisGlass::TetrisGlass(QGraphicsItem *parent)
    : QGraphicsItem (parent)
{

}

QRectF TetrisGlass::boundingRect() const
{
    return QRectF(m_left, m_top, m_width, m_height);
}

void TetrisGlass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
    painter->setPen(QPen(Qt::darkGreen, 1, Qt::SolidLine, Qt::FlatCap));
    painter->drawRect(boundingRect());
}

void TetrisGlass::setGeometry(const qreal sceneWidth, const qreal sceneHeight, const int h, const int v)
{
    m_height = sceneHeight;
    qreal s = (sceneHeight - m_glassBorder) / h;
    m_width = (s * v) + m_glassBorder * 2;
    m_top = 0;
    m_left = sceneWidth / 2 - m_width / 2;
}

//void TetrisGlass::setGeometry(const qreal left, const qreal top, const qreal width, const qreal height)
//{
//    m_left = left;
//    m_top = top;
//    m_width = width;
//    m_height = height;
//}

