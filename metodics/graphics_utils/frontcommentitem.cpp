#include "frontcommentitem.h"

#include <QPainter>

FrontCommentItem::FrontCommentItem(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsItem (parent)
    , m_rect(rect)
{

}

QRectF FrontCommentItem::boundingRect() const
{
    return m_rect;
}

void FrontCommentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(QColor(180, 0, 0), 1, Qt::SolidLine, Qt::FlatCap));
    painter->setFont(QFont("Bookman Old Style", 68, QFont::Bold, true));
    painter->drawText(m_rect, Qt::AlignHCenter | Qt::AlignVCenter, m_text);
}

void FrontCommentItem::setText(const QString &text)
{
    m_text = text;
    setVisible(text != "");
}
