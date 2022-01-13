#include "figures.h"
#include "ui_figures.h"

#include <QPainter>

Figures::Figures(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Figures)
{
    ui->setupUi(this);
}

Figures::~Figures()
{
    delete ui;
}

void Figures::setSize(const int size)
{
    m_size = size;
    update();
}

void Figures::setFigureType(const Figures::FigureType ft)
{
    m_ft = ft;
    update();
}

void Figures::setColor(const QColor color)
{
    m_color = color;
    update();
}

QColor Figures::color() const
{
    return m_color;
}

void Figures::setBackgroundColor(const QColor color)
{
    m_backgroundColor = color;
}

QColor Figures::backgroundColor() const
{
    return m_backgroundColor;
}

void Figures::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    painter.setBrush(QBrush(m_color, Qt::SolidPattern));
    painter.setPen(QPen(m_color, 1, Qt::SolidLine, Qt::FlatCap));

    int mx = geometry().width() / 2;
    int my = geometry().height() / 2;

    switch (m_ft)
    {
    case ftEllipse:
        painter.drawEllipse(mx - m_size / 2, my - m_size / 2, m_size, m_size);
        break;
    case ftRect:
        painter.drawRect(mx - m_size / 2, my - m_size / 2, m_size, m_size);
        break;
    default:
        break;
    }
}
