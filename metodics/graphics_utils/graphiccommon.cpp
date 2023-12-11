#include "graphiccommon.h"

#include "baseutils.h"
#include <QPainter>

GraphicCommon::BackgroundElement::BackgroundElement(const GraphicCommon::BackgroundMode mode, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    m_mode = mode;
    m_rect.setWidth(100);
    m_rect.setHeight(100);
}

QRectF GraphicCommon::BackgroundElement::boundingRect() const
{
    return m_rect;
}

void GraphicCommon::BackgroundElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRect rect(static_cast<int>(boundingRect().x()), static_cast<int>(boundingRect().y()),
               static_cast<int>(boundingRect().width()), static_cast<int>(boundingRect().height()));

    if (m_mode == bkgmPicture)
    {
        painter->drawPixmap(rect, m_pixmap);
    }
    else
    if (m_mode == bkgmPlate)
    {
        int x = static_cast<int>(boundingRect().x());
        while (x < boundingRect().width())
        {
            int y = static_cast<int>(boundingRect().y());
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

void GraphicCommon::BackgroundElement::assignPixmap(const QString &fnPixmap)
{
    m_pixmap.load(fnPixmap);
}

GraphicCommon::GameElement::GameElement(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    m_size.setWidth(50);
    m_size.setHeight(50);
    m_code = 0;
}

QRectF GraphicCommon::GameElement::boundingRect() const
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

void GraphicCommon::GameElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRect rect(static_cast<int>(boundingRect().x()), static_cast<int>(boundingRect().y()),
               static_cast<int>(boundingRect().width()), static_cast<int>(boundingRect().height()));

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

    if (m_presentTime > 0)
    {
        painter->save();
        painter->setPen(QPen(Qt::yellow));
        auto font = painter->font();
        font.setPixelSize(40);
        font.setBold(true);
        painter->setFont(font);
        auto s = QString::number(m_presentTime - m_timeCounter);
        if (s.size() > 6)
            s = "0";
        painter->drawText(static_cast<int>(boundingRect().x() + 20), static_cast<int>(boundingRect().y() + 20), s);
        painter->restore();
    }
}

void GraphicCommon::GameElement::assignElementInfo(GraphicCommon::GameElementInfo *element,
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

    m_timeCounter = 0;
    if (m_element->timeLimitMin > 0 && m_element->timeLimitMax > 0)
    {
        if (m_element->timeLimitMax > m_element->timeLimitMin)
            m_presentTime = m_element->timeLimitMin + qrand() % (m_element->timeLimitMax - m_element->timeLimitMin);
        else
            m_presentTime = m_element->timeLimitMin;
    }
    else
        m_presentTime = 0;
}

int GraphicCommon::GameElement::code() const
{
    if (m_element->code == -1)
        return m_code;
    else
        return m_element->code;
}


void GraphicCommon::MarkerElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (m_isShotTrace)
    {
        if (m_traceCounter / 2 % 2 == 0)
        {
            painter->save();
            auto pen = painter->pen();
            pen.setStyle(Qt::DashDotDotLine);
            pen.setWidth(3);
            pen.setColor(Qt::yellow);
            painter->setPen(pen);

            painter->drawLine(boundingRect().bottomLeft(), boundingRect().center());
            painter->drawLine(boundingRect().bottomRight(), boundingRect().center());
            painter->restore();

        }
        ++m_traceCounter;
    }

    QGraphicsPixmapItem::paint(painter, option, widget);
}

void GraphicCommon::MarkerElement::setShotTrace(const bool isShotTrace)
{
    if (isShotTrace && !m_isShotTrace)
        m_traceCounter = 0;
    m_isShotTrace = isShotTrace;
}

//GraphicCommon::VidioIrritant::VidioIrritant(const QRectF &rect, QGraphicsItem *parent)
//    : QGraphicsItem (parent)
//{
//    m_rect = rect;
//    setOpacity(0.5);
//}

//QRectF GraphicCommon::VidioIrritant::boundingRect() const
//{
//    return m_rect;
//}

//void GraphicCommon::VidioIrritant::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    Q_UNUSED(option);
//    Q_UNUSED(widget);

//    QRect rect(static_cast<int>(boundingRect().x()), static_cast<int>(boundingRect().y()),
//               static_cast<int>(boundingRect().width() / 2), static_cast<int>(boundingRect().height() / 2));

//    painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));
//    painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
//    painter->drawRect(rect);

//}
