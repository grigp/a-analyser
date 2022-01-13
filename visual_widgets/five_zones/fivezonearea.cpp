#include "fivezonearea.h"
#include "ui_fivezonearea.h"

#include <QPainter>
#include <QDebug>

namespace  {
static int ZenterSize = 40;

}

FiveZoneArea::FiveZoneArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FiveZoneArea)
{
    ui->setupUi(this);
}

FiveZoneArea::~FiveZoneArea()
{
    delete ui;
}

void FiveZoneArea::setDiap(const int diap)
{
    m_diap = diap;
    update();
}

int FiveZoneArea::diap() const
{
    return m_diap;
}

void FiveZoneArea::setMarker(const double x, const double y)
{
    m_x = x;
    m_y = y;
    update();
}

void FiveZoneArea::setBackgroundColor(const QColor color)
{
    m_backgroundColor = color;
    update();
}

QColor FiveZoneArea::backgroundColor() const
{
    return m_backgroundColor;
}

void FiveZoneArea::setGridColor(const QColor color)
{
    m_gridColor = color;
    update();
}

QColor FiveZoneArea::gridColor() const
{
    return m_gridColor;
}

void FiveZoneArea::setMarkerColor(const QColor color)
{
    m_markerColor = color;
    update();
}

QColor FiveZoneArea::markerColor() const
{
    return m_markerColor;
}

void FiveZoneArea::setMarkerSize(const int size)
{
    m_markerSize = size;
    update();
}

int FiveZoneArea::markerSize() const
{
    return m_markerSize;
}

void FiveZoneArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    //! Фон
    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    //! Размеры
    int size = geometry().height();
    if (geometry().height() > geometry().width())
        size = geometry().width();
    int midX = geometry().width() / 2;
    int midY = geometry().height() / 2;

    //! Сетка
    painter.setPen(QPen(m_gridColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(midX - ZenterSize * (size / 2) / 128, midY - ZenterSize * (size / 2) / 128,
                     ZenterSize * 2 * (size / 2) / 128, ZenterSize * 2 * (size / 2) / 128);
    painter.drawLine(midX - ZenterSize * (size / 2) / 128, midY - ZenterSize * (size / 2) / 128,
                     midX - size / 2, midY - size / 2);
    painter.drawLine(midX - ZenterSize * (size / 2) / 128, midY + ZenterSize * (size / 2) / 128,
                     midX - size / 2, midY + size / 2);
    painter.drawLine(midX + ZenterSize * (size / 2) / 128, midY - ZenterSize * (size / 2) / 128,
                     midX + size / 2, midY - size / 2);
    painter.drawLine(midX + ZenterSize * (size / 2) / 128, midY + ZenterSize * (size / 2) / 128,
                     midX + size / 2, midY + size / 2);

    painter.setBrush(QBrush(m_markerColor, Qt::SolidPattern));
    painter.setPen(QPen(m_markerColor, 1, Qt::SolidLine, Qt::FlatCap));
    int d = (midY - ZenterSize * (size / 2) / 128) / 2;

    if (fabs(m_y) > fabs(m_x) && m_y > m_diap * ZenterSize / 128)
        painter.drawRect(midX - m_markerSize / 2, midY - size / 2 + d - m_markerSize / 2, m_markerSize, m_markerSize); //! Верх
    else
    if (fabs(m_y) > fabs(m_x) && m_y < - m_diap * ZenterSize / 128)
        painter.drawRect(midX - m_markerSize / 2, midY + size / 2 - d - m_markerSize / 2, m_markerSize, m_markerSize); //! Низ
    else
    if (fabs(m_x) > fabs(m_y) && m_x < - m_diap * ZenterSize / 128)
        painter.drawRect(midX - size / 2 + d - m_markerSize / 2, midY - m_markerSize / 2, m_markerSize, m_markerSize); //! Лево
    else
    if (fabs(m_x) > fabs(m_y) && m_x > m_diap * ZenterSize / 128)
        painter.drawRect(midX + size / 2 - d - m_markerSize / 2, midY - m_markerSize / 2, m_markerSize, m_markerSize); //! Право
}
