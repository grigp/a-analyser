#include "targetarea.h"
#include "ui_targetarea.h"

#include <QPainter>
#include <QFontMetrics>

TargetArea::TargetArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TargetArea)
{
    ui->setupUi(this);
}

TargetArea::~TargetArea()
{
    delete ui;
}

void TargetArea::setDiap(const int diap)
{
    m_diap = diap;
}

int TargetArea::diap() const
{
    return m_diap;
}

void TargetArea::setMarker(const double x, const double y)
{
    m_x = x;
    m_y = y;
    repaint();
}

void TargetArea::setBackgroundColor(const QColor color)
{
    m_backgroundColor = color;
}

QColor TargetArea::backgroundColor() const
{
    return m_backgroundColor;
}

void TargetArea::setSpaceColor(const QColor color)
{
    m_spaceColor = color;
}

QColor TargetArea::spaceColor() const
{
    return m_spaceColor;
}

void TargetArea::setCenterColor(const QColor color)
{
    m_centerColor = color;
}

QColor TargetArea::centerColor() const
{
    return m_centerColor;
}

void TargetArea::setMarkerColor(const QColor color)
{
    m_markerColor = color;
}

QColor TargetArea::markerColor() const
{
    return m_markerColor;
}

void TargetArea::setMarkerSize(const int size)
{
    m_markerSize = size;
}

int TargetArea::markerSize() const
{
    return m_markerSize;
}

void TargetArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    //! Фон
    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    //! Мишень
    auto font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);

    int radius = geometry().width() / 2;
    if (geometry().width() > geometry().height())
        radius = geometry().height() / 2;

    for (int i = 0; i < 11; ++i)
    {
        if (i < 6)
        {
            painter.setBrush(QBrush(m_spaceColor, Qt::SolidPattern));
            painter.setPen(QPen(m_centerColor, 1, Qt::SolidLine, Qt::FlatCap));
        }
        else
        {
            painter.setBrush(QBrush(m_centerColor, Qt::SolidPattern));
            painter.setPen(QPen(m_spaceColor, 1, Qt::SolidLine, Qt::FlatCap));

        }

        if (i < 10)
            painter.drawEllipse(geometry().width() / 2 - radius / 10 * (10 - i),
                                geometry().height() / 2 - radius / 10 * (10 - i),
                                radius / 10 * (10 - i) * 2,
                                radius / 10 * (10 - i) * 2);
        else
            // Последний круг с в три раза меньшим уменьшением радиуса
            painter.drawEllipse(geometry().width() / 2 - radius / 30 * (30 - 29),
                                geometry().height() / 2 - radius / 30 * (30 - 29),
                                radius / 30 * (30 - 29) * 2,
                                radius / 30 * (30 - 29) * 2);

        if (i < 8)
        {
            auto label = QString::number(i + 1);
            QFontMetrics fm(painter.font());
            auto lw = fm.width(label);
            auto lh = fm.height();
            painter.drawText(geometry().width() / 2 - lw / 2,
                             geometry().height() / 2 - radius / 10 * (10 - i) + radius / 20 + lh / 2,
                             label);
            painter.drawText(geometry().width() / 2 - lw / 2,
                             geometry().height() / 2 + radius / 10 * (10 - i) - radius / 20 + lh / 2,
                             label);
            painter.drawText(geometry().width() / 2 - radius / 10 * (10 - i) + radius / 20 - lw / 2,
                             geometry().height() / 2 + lh / 2,
                             label);
            painter.drawText(geometry().width() / 2 + radius / 10 * (10 - i) - radius / 20 - lw / 2,
                             geometry().height() / 2 + lh / 2,
                             label);
        }
    }

    //! Маркер
    painter.setBrush(QBrush(m_markerColor, Qt::SolidPattern));
    painter.setPen(QPen(m_markerColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry().width() / 2 + m_x / m_diap * radius - m_markerSize / 2,
                     geometry().height() / 2 - m_y / m_diap * radius - m_markerSize / 2,
                     m_markerSize, m_markerSize);
}
