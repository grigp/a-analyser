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

//    painter->setPen(QPen(m_color, 1, Qt::SolidLine, Qt::FlatCap));
//    painter->setBrush(QBrush(m_color, Qt::SolidPattern));

    int msec = m_time.msecsTo(QTime::currentTime());
    m_time = QTime::currentTime();
    auto speed = static_cast<double>(m_speed) * static_cast<double>(msec) / 1000.0;

    const int iGradientSize = m_width / 15;
    painter->setPen(QPen(m_color, 1, Qt::NoPen, Qt::FlatCap));

    //! Рисование горизонтальных градиентов по краям полосы
    auto horizontalGradient = [&](const qreal x)
    {
        QLinearGradient grad1(QPointF(x, boundingRect.y()), QPointF(x - iGradientSize, boundingRect.y()));
        grad1.setColorAt(0, QColor(m_color.red(), m_color.green(), m_color.blue(), static_cast<int>(0.5 * 255)));
        grad1.setColorAt(1, QColor(m_color.red(), m_color.green(), m_color.blue(), 0));
        painter->setBrush(grad1);
        painter->drawRect(static_cast<int>(x - iGradientSize), static_cast<int>(boundingRect.y()),
                          iGradientSize, static_cast<int>(boundingRect.height()));

        QLinearGradient grad2(QPointF(x + m_width, boundingRect.y()), QPointF(x + m_width + iGradientSize, boundingRect.y()));
        grad2.setColorAt(0, QColor(m_color.red(), m_color.green(), m_color.blue(), static_cast<int>(0.5 * 255)));
        grad2.setColorAt(1, QColor(m_color.red(), m_color.green(), m_color.blue(), 0));
        painter->setBrush(grad2);
        painter->drawRect(static_cast<int>(x) + m_width, static_cast<int>(boundingRect.y()),
                          iGradientSize, static_cast<int>(boundingRect.height()));
    };

    //! Рисование вертикальных градиентов по краям полосы
    auto verticalGradient = [&](const qreal y)
    {
        QLinearGradient grad1(QPointF(boundingRect.x(), y), QPointF(boundingRect.x(), y - iGradientSize));
        grad1.setColorAt(0, QColor(m_color.red(), m_color.green(), m_color.blue(), static_cast<int>(0.5 * 255)));
        grad1.setColorAt(1, QColor(m_color.red(), m_color.green(), m_color.blue(), 0));
        painter->setBrush(grad1);
        painter->drawRect(static_cast<int>(boundingRect.x()), static_cast<int>(y - iGradientSize),
                          static_cast<int>(boundingRect.width()), iGradientSize);

        QLinearGradient grad2(QPointF(boundingRect.x(), y + m_width), QPointF(boundingRect.x(), y + m_width + iGradientSize));
        grad2.setColorAt(0, QColor(m_color.red(), m_color.green(), m_color.blue(), static_cast<int>(0.5 * 255)));
        grad2.setColorAt(1, QColor(m_color.red(), m_color.green(), m_color.blue(), 0));
        painter->setBrush(grad2);
        painter->drawRect(static_cast<int>(boundingRect.x()), static_cast<int>(y) + m_width,
                          static_cast<int>(boundingRect.width()), iGradientSize);
    };

//    qDebug() << msec << speed << "   " << iGradientSize << m_width / 15;

    if (m_direction == BaseDefines::dirRight)
    {
        auto x = boundingRect.x() - m_width * (m_dutyCycle + 1) + static_cast<int>(m_position) % (m_width * (m_dutyCycle + 1));
        while (x < boundingRect.x() + boundingRect.width())
        {
            painter->setBrush(QBrush(m_color, Qt::SolidPattern));
            painter->drawRect(static_cast<int>(x), static_cast<int>(boundingRect.y()),
                              m_width, static_cast<int>(boundingRect.height()));
            horizontalGradient(x);

            x += m_width * (m_dutyCycle + 1);
        }
    }
    else
    if (m_direction == BaseDefines::dirLeft)
    {
        auto x = boundingRect.x() + boundingRect.width() + m_width * (m_dutyCycle + 1) - static_cast<int>(m_position) % (m_width * (m_dutyCycle + 1));
        while (x > boundingRect.x() - m_width * (m_dutyCycle + 1))
        {
            painter->setBrush(QBrush(m_color, Qt::SolidPattern));
            painter->drawRect(static_cast<int>(x), static_cast<int>(boundingRect.y()),
                              m_width, static_cast<int>(boundingRect.height()));
            horizontalGradient(x);

            x -= m_width * (m_dutyCycle + 1);
        }
    }
    else
    if (m_direction == BaseDefines::dirDown)
    {
        auto y = boundingRect.y() - m_width * (m_dutyCycle + 1) + static_cast<int>(m_position) % (m_width * (m_dutyCycle + 1));
        while (y < boundingRect.y() + boundingRect.height())
        {
            painter->setBrush(QBrush(m_color, Qt::SolidPattern));
            painter->drawRect(static_cast<int>(boundingRect.x()), static_cast<int>(y),
                              static_cast<int>(boundingRect.width()), m_width);
            verticalGradient(y);

            y += m_width * (m_dutyCycle + 1);
        }
    }
    else
    if (m_direction == BaseDefines::dirUp)
    {
        auto y = boundingRect.y() + boundingRect.height() + m_width * (m_dutyCycle + 1) - static_cast<int>(m_position) % (m_width * (m_dutyCycle + 1));
        while (y > boundingRect.y() - m_width * (m_dutyCycle + 1))
        {
            painter->setBrush(QBrush(m_color, Qt::SolidPattern));
            painter->drawRect(static_cast<int>(boundingRect.x()), static_cast<int>(y),
                              static_cast<int>(boundingRect.width()), m_width);
            verticalGradient(y);

            y -= m_width * (m_dutyCycle + 1);
        }
    }

    m_position += speed;
}

QWidget* LinesIrriant::getSettingsWidget()
{
    return new LinesIrriantSettingsWidget(this);
}

