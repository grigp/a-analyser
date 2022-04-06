#include "platespositioning.h"
#include "ui_platespositioning.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

PlatesPositioning::PlatesPositioning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlatesPositioning)
{
    ui->setupUi(this);
}

PlatesPositioning::~PlatesPositioning()
{
    delete ui;
}

void PlatesPositioning::setBackgroundColor(const QColor color)
{
    m_backgroundColor = color;
    update();
}

QColor PlatesPositioning::backgroundColor() const
{
    return m_backgroundColor;
}

void PlatesPositioning::setPlatformColor(const QColor color)
{
    m_platformsColor = color;
    update();
}

QColor PlatesPositioning::platformColor() const
{
    return m_platformsColor;
}

void PlatesPositioning::setFontColor(const QColor color)
{
    m_fontColor = color;
    update();
}

QColor PlatesPositioning::fontColor() const
{
    return m_fontColor;
}

void PlatesPositioning::setPlate1Size(const int w, const int h)
{
    m_plate1.setSize(QSize(w, h));
    update();
}

void PlatesPositioning::setPlate2Size(const int w, const int h)
{
    m_plate2.setSize(QSize(w, h));
    update();
}

void PlatesPositioning::setPlate1(const int x, const int y)
{
    m_plate1.moveTo(x, y);
    update();
}

void PlatesPositioning::setPlate2(const int x, const int y)
{
    m_plate2.moveTo(x, y);
    update();
}

void PlatesPositioning::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    painter.save();

    //! Фон
    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    //! Размер условной 500 мм платформы
    int size = 0;
    if (geometry().width() < geometry().height())
        size = geometry().width() / 4;
    else
        size = geometry().height() / 4;
    m_prop = static_cast<double>(size) / 500.0;

    //! Координаты точки 0
    m_x0 = size / 2;
    m_y0 = geometry().height() - size / 2;


    painter.setBrush(QBrush(m_platformsColor, Qt::SolidPattern));
    painter.setPen(QPen(m_fontColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setFont(QFont("Sans", 18, QFont::Bold, false));

    m_rectPlate1 = QRect(m_x0 + static_cast<int>(m_plate1.x() * m_prop),
                         m_y0 - static_cast<int>(m_plate1.y() * m_prop),
                         static_cast<int>(m_plate1.width() * m_prop),
                         static_cast<int>(m_plate1.height() * m_prop));
    painter.drawRect(m_rectPlate1);
    painter.drawText(m_rectPlate1, "1");

    m_rectPlate2 = QRect(m_x0 + static_cast<int>(m_plate2.x() * m_prop),
                         m_y0 - static_cast<int>(m_plate2.y() * m_prop),
                         static_cast<int>(m_plate2.width() * m_prop),
                         static_cast<int>(m_plate2.height() * m_prop));
    painter.drawRect(m_rectPlate2);
    painter.drawText(m_rectPlate2, "2");


    painter.restore();
}

void PlatesPositioning::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if (m_rectPlate1.contains(event->x(), event->y()))
    {
        m_dx = event->x() - (m_x0 + static_cast<int>(m_plate1.x() * m_prop));
        m_dy = event->y() - (m_y0 - static_cast<int>(m_plate1.y() * m_prop));
        m_selectedPlate = 0;
    }
    else
    if (m_rectPlate2.contains(event->x(), event->y()))
    {
        m_dx = event->x() - (m_x0 + static_cast<int>(m_plate2.x() * m_prop));
        m_dy = event->y() - (m_y0 - static_cast<int>(m_plate2.y() * m_prop));
        m_selectedPlate = 1;
    }
    else
        m_selectedPlate = -1;
}

void PlatesPositioning::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_selectedPlate = -1;
}

void PlatesPositioning::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    if (m_rectPlate1.contains(event->x(), event->y()))
    {
        setCursor(QCursor(Qt::SizeVerCursor));
        if (m_selectedPlate == 0)
            emit movePlate(0, m_plate1.x(), static_cast<int>((m_y0 + m_dy - event->y()) / m_prop));
    }
    else
    if (m_rectPlate2.contains(event->x(), event->y()))
    {
        setCursor(QCursor(Qt::SizeAllCursor));
        if (m_selectedPlate == 1)
        {
            int x = static_cast<int>((event->x() - m_dx - m_x0) / m_prop);
            if (x < 0)
                x = 0;
            emit movePlate(1, x, static_cast<int>((m_y0 + m_dy - event->y()) / m_prop));
        }
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
}
