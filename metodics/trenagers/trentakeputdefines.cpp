#include "trentakeputdefines.h"

#include "baseutils.h"

#include <QPainter>
#include <QDebug>

TrenTakePutDefines::GameElement::GameElement(const QString name, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_name(name)
{
    m_size.setWidth(50);
    m_size.setHeight(50);
    m_code = 0;
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

    if (m_element->style == esPictureFixed || m_element->style == esPictureSplit ||
        m_element->style == esPicturePair || m_element->style == esPictureRandom)
        painter->drawPixmap(rect, m_pixmap);
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
    if (m_element->style == esPictureFixed || m_element->style == esPictureRandom ||
        m_element->style == esPictureSplit || m_element->style == esPicturePair)
    {
        if (m_element->style == esPictureFixed)
            m_pixmap.load(":/images/Games/" + m_element->images);
        else
        if (m_element->style == esPictureSplit || m_element->style == esPicturePair ||
            m_element->style == esPictureRandom)
            if (pixmap)
                m_pixmap = *pixmap;
        if (m_element->isRepaint)
            BaseUtils::setColoredPicture(m_pixmap, m_element->color);
    }
    if (m_element->presentTime > 0)
        m_elapsedTime = m_element->presentTime;
}

int TrenTakePutDefines::GameElement::code() const
{
    if (m_element->code == -1)
        return m_code;
    else
        return m_element->code;
}


TrenTakePutDefines::BackgroundElement::BackgroundElement(const BackgroundMode mode, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    m_mode = mode;
    m_rect.setWidth(100);
    m_rect.setHeight(100);
}

QRectF TrenTakePutDefines::BackgroundElement::boundingRect() const
{
    return m_rect;
}

void TrenTakePutDefines::BackgroundElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRect rect(boundingRect().x(), boundingRect().y(),
               boundingRect().width(), boundingRect().height());

    if (m_mode == bkgmPicture)
    {
        painter->drawPixmap(rect, m_pixmap);
    }
    else
    if (m_mode == bkgmPlate)
    {
        int x = boundingRect().x();
        while (x < boundingRect().width())
        {
            int y = boundingRect().y();
            while(y < boundingRect().height())
            {
                painter->drawPixmap(x, y, m_pixmap);
                y += m_pixmap.height();
            }
            x += m_pixmap.width();
        }
    }
    if (m_mode == bkgmDrawing)
    {

    }
}

void TrenTakePutDefines::BackgroundElement::assignPixmap(const QString &fnPixmap)
{
    m_pixmap.load(fnPixmap);
}
