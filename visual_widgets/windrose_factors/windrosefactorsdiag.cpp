#include "windrosefactorsdiag.h"
#include "ui_windrosefactorsdiag.h"

#include <QPainter>
#include <QDebug>

namespace  {
///< Отступы от краев виджета
static const int LeftSpace = 10;
static const int RightSpace = 10;
static const int TopSpace = 10;
static const int BottomSpace = 10;
}



WindRoseFactorsDiag::WindRoseFactorsDiag(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindRoseFactorsDiag)
{
    ui->setupUi(this);

    m_factors.clear();
}

WindRoseFactorsDiag::~WindRoseFactorsDiag()
{
    delete ui;
}

void WindRoseFactorsDiag::addFactor(const double value)
{
    m_factors << value;
}

void WindRoseFactorsDiag::clear()
{
    m_factors.clear();
}

void WindRoseFactorsDiag::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.save();

    //! Фон
    auto backColor = palette().background().color();
    painter.setBrush(QBrush(backColor, Qt::SolidPattern));
    painter.setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(0, 0, width(), height());

    QRect diagRect;
    int size = 0;
    if (width() - LeftSpace - RightSpace < height() - TopSpace - BottomSpace)
    {
        size = width() - LeftSpace - RightSpace;
        diagRect = QRect(LeftSpace, height() / 2 - size / 2, size,size);
    }
    else
    {
        size = height() - TopSpace - BottomSpace;
        diagRect = QRect(width() / 2 - size / 2, TopSpace, size, size);
    }

    //! Цветовая диаграмма нормы
    int r = 255;
    int g = 0;
    int b = 0;
    size /= 2;
    painter.setBrush(QBrush(Qt::NoBrush));
    for (int i = 0; i < size; ++i)
    {
        if (i < size / 2)
        {
            g = static_cast<int>(static_cast<double>(i) / static_cast<double>(size / 2) * 255);
        }
        else
        {
            r = static_cast<int>(255 - (i - size / 2) / static_cast<double>(size / 2) * 255);
        }
        painter.setPen(QPen(QColor(r, g, b), 2, Qt::SolidLine, Qt::FlatCap));
        painter.drawEllipse(diagRect.center(), i, i);
    }

    //! Оси показателей
    QPoint diag (diagRect.center());
    QPoint diagPrev (diagRect.center());
    QPoint diagFirst (diagRect.center());
    for (int i = 0; i < m_factors.size(); ++i)
    {
        QPoint end (diagRect.center());
        double angle = (2 * M_PI) / m_factors.size() * i;
        double value = (m_factors.at(i) - m_min) / (m_max - m_min) * size;
        if (angle < M_PI_2)
        {
            end.setX(diagRect.center().x() - static_cast<int>(size * sin(angle)));
            end.setY(diagRect.center().y() - static_cast<int>(size * cos(angle)));
            diag.setX(diagRect.center().x() - static_cast<int>(value * sin(angle)));
            diag.setY(diagRect.center().y() - static_cast<int>(value * cos(angle)));
        }
        else
        if (angle < M_PI)
        {
            end.setX(diagRect.center().x() - static_cast<int>(size * cos(angle - M_PI_2)));
            end.setY(diagRect.center().y() + static_cast<int>(size * sin(angle - M_PI_2)));
            diag.setX(diagRect.center().x() - static_cast<int>(value * cos(angle - M_PI_2)));
            diag.setY(diagRect.center().y() + static_cast<int>(value * sin(angle - M_PI_2)));
        }
        else
        if (angle < 3 * M_PI / 2)
        {
            end.setX(diagRect.center().x() + static_cast<int>(size * sin(angle - M_PI)));
            end.setY(diagRect.center().y() + static_cast<int>(size * cos(angle - M_PI)));
            diag.setX(diagRect.center().x() + static_cast<int>(value * sin(angle - M_PI)));
            diag.setY(diagRect.center().y() + static_cast<int>(value * cos(angle - M_PI)));
        }
        else
        {
            end.setX(diagRect.center().x() + static_cast<int>(size * cos(angle - 3 * M_PI / 2)));
            end.setY(diagRect.center().y() - static_cast<int>(size * sin(angle - 3 * M_PI / 2)));
            diag.setX(diagRect.center().x() + static_cast<int>(value * sin(angle - M_PI)));
            diag.setY(diagRect.center().y() + static_cast<int>(value * cos(angle - M_PI)));
        }

        painter.setPen(QPen(m_axisColor, 1, Qt::SolidLine, Qt::FlatCap));
        painter.drawLine(diagRect.center(), end);

        painter.setPen(QPen(m_lineColor, 2, Qt::SolidLine, Qt::FlatCap));
        if (i > 0)
            painter.drawLine(diagPrev, diag);
        else
            diagFirst = diag;
        diagPrev = diag;
    }
    painter.drawLine(diag, diagFirst);

    painter.restore();
}
