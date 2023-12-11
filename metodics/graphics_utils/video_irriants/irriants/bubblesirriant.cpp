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

    painter->setPen(QPen(m_color, 1, Qt::NoPen, Qt::FlatCap));
    for (int i = 0; i < m_bubbles.size(); ++i)
    {
        auto bubble = m_bubbles.at(i);
        painter->setBrush(QBrush(QColor(m_color.red(), color().green(), color().blue(), bubble.alpha), Qt::SolidPattern));
        painter->drawEllipse(static_cast<int>(boundingRect.x() + boundingRect.width() * bubble.x),
                             static_cast<int>(boundingRect.y() + boundingRect.height() * bubble.y),
                             bubble.size, bubble.size);

        bubble.alpha += bubble.chAlpha;
        if (bubble.alpha > 255)
        {
            bubble.alpha = 255 - (bubble.alpha - 255);
            bubble.chAlpha = - bubble.chAlpha;
        }
        else
        if (bubble.alpha < 0)
        {
            bubble.alpha = - bubble.alpha;
            bubble.chAlpha = - bubble.chAlpha;
        }

        m_bubbles.replace(i, bubble);
    }
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

void BubblesIrriant::setMaxSize(const int maxSize)
{
    m_maxSize = maxSize;
    initBubbles();
}

void BubblesIrriant::initBubbles()
{
    m_bubbles.clear();
    for (int i = 0; i < m_bubblesCount; ++i)
    {
        m_bubbles.append(BubbleInfo(qrand() % m_maxSize,
                                    static_cast<double>((qrand() % 1000)) / 1000.0,
                                    static_cast<double>((qrand() % 1000)) / 1000.0,
                                    qrand() % 200,
                                    qrand() % 8 - 4));
    }
}
