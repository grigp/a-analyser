#include "trentakeputdefines.h"

#include "baseutils.h"

#include <QPainter>
#include <QDebug>

TrenTakePutDefines::GameElement::GameElement(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{

}

QRectF TrenTakePutDefines::GameElement::boundingRect() const
{
    if (m_pixmap.width() == 0 || m_pixmap.height() == 0)
        return QRectF(-25, -25, 50, 50);
    else
//        return QRectF(-25, -25, m_pixmap.width(), m_pixmap.height());
        return QRectF(0, 0, m_pixmap.width(), m_pixmap.height());
}

void TrenTakePutDefines::GameElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRect rect(boundingRect().x(), boundingRect().y(),
               boundingRect().width(), boundingRect().height());
    painter->drawPixmap(rect, m_pixmap);
}

void TrenTakePutDefines::GameElement::assignElementInfo(TrenTakePutDefines::GameElementInfo *element)
{
    m_element = element;
    if (m_element->style == esPictureFixed)
    {
        m_pixmap.load(":/images/Games/" + m_element->images);
        if (m_element->isRepaint)
            BaseUtils::setColoredPicture(m_pixmap, m_element->color);
    }
}

