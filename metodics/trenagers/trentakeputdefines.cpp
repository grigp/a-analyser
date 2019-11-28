#include "trentakeputdefines.h"

#include "baseutils.h"

#include <QPainter>
#include <QDebug>

TrenTakePutDefines::GameElement::GameElement(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    m_size.setWidth(50);
    m_size.setHeight(50);
}

QRectF TrenTakePutDefines::GameElement::boundingRect() const
{
    if (m_element->style == esPictureFixed || m_element->style == esPictureSplit || m_element->style == esPicturePair)
    {
        if (m_pixmap.width() != 0 || m_pixmap.height() != 0)
            return QRectF(0, 0, m_pixmap.width(), m_pixmap.height());
    }
    else
    if (m_element->style == esDrawing)
    {
        return QRectF(0, 0, m_size.width(), m_size.height());

    }
    return QRectF(-25, -25, m_size.width(), m_size.height());
}

void TrenTakePutDefines::GameElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRect rect(boundingRect().x(), boundingRect().y(),
               boundingRect().width(), boundingRect().height());

    if (m_element->style == esPictureFixed || m_element->style == esPictureSplit || m_element->style == esPicturePair)
    {
        painter->drawPixmap(rect, m_pixmap);
    }
    else
    if (m_element->style == esDrawing)
    {
        painter->setBrush(QBrush(m_element->color));
        if (m_element->drawing == edRectangle)
            painter->drawRect(rect);
        else
        if (m_element->drawing == edCircle)
            painter->drawEllipse(rect);
    }
}

void TrenTakePutDefines::GameElement::assignElementInfo(TrenTakePutDefines::GameElementInfo *element,
                                                        const QPixmap *pixmap)
{
    m_element = element;
    if (m_element->style == esPictureFixed || m_element->style == esPictureSplit || m_element->style == esPicturePair)
    {
        if (m_element->style == esPictureFixed)
            m_pixmap.load(":/images/Games/" + m_element->images);
        else
        if (m_element->style == esPictureSplit)
            if (pixmap)
                m_pixmap = *pixmap;
        if (m_element->isRepaint)
            BaseUtils::setColoredPicture(m_pixmap, m_element->color);
    }
}

