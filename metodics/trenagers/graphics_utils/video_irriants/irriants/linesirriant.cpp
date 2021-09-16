#include "linesirriant.h"

#include "linesirriantsettingswidget.h"

#include <QPainter>
#include <QDebug>

LinesIrriant::LinesIrriant()
    : Irriant ()
{

}

void LinesIrriant::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF boundingRect)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(m_color, 1, Qt::SolidLine, Qt::FlatCap));
    painter->setBrush(QBrush(m_color, Qt::SolidPattern));

//    int msec = m_time.msecsTo(QTime::currentTime());
//    m_time = QTime::currentTime();
//    qDebug() << msec;

    if (m_direction == BaseUtils::dirRight)
    {
        auto x = boundingRect.x() - m_width * (m_dutyCycle + 1) + m_position % (m_width * (m_dutyCycle + 1));
        while (x < boundingRect.x() + boundingRect.width())
        {
            painter->drawRect(static_cast<int>(x), static_cast<int>(boundingRect.y()),
                              m_width, static_cast<int>(boundingRect.height()));

            x += m_width * (m_dutyCycle + 1);
        }
    }

    m_position += m_speed;
}

QWidget* LinesIrriant::getSettingsWidget()
{
    return new LinesIrriantSettingsWidget(this);
}

