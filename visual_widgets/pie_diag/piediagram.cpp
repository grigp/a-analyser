#include "piediagram.h"
#include "ui_piediagram.h"

#include <QPainter>

namespace  {
///< Отступы от краев виджета
static const int LeftSpace = 5;
static const int RightSpace = 5;
static const int TopSpace = 5;
static const int BottomSpace = 5;
}

PieDiagram::PieDiagram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PieDiagram)
{
    ui->setupUi(this);
}

PieDiagram::~PieDiagram()
{
    delete ui;
}

void PieDiagram::addFactor(const double value, const QColor &color)
{
    m_factors << Value(value, color);
}

void PieDiagram::clear()
{
    m_factors.clear();
}

void PieDiagram::paintEvent(QPaintEvent *event)
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

    size /= 2;

    //! Расчет процентов в круге
    double summ = 0;
    for (int i = 0; i < m_factors.size(); ++i)
        summ += m_factors.at(i).value;
    for (int i = 0; i < m_factors.size(); ++i)
    {
        auto fi = m_factors.at(i);
        fi.percent = m_factors.at(i).value / summ * 100;
        m_factors.replace(i, fi);
    }

    //! Сама диаграмма
    int startAngle = 90 * 16;
    for (int i = 0; i < m_factors.size(); ++i)
    {
        painter.setBrush(QBrush(m_factors.at(i).color, Qt::SolidPattern));
        painter.setPen(QPen(m_factors.at(i).color, 1, Qt::SolidLine, Qt::FlatCap));

        int spanAngle = static_cast<int>(m_factors.at(i).percent / 100 * 5760);
        painter.drawPie(diagRect, startAngle, spanAngle);

        startAngle += spanAngle;
    }

    painter.setBrush(QBrush(backColor, Qt::SolidPattern));
    painter.setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawEllipse(diagRect.center().x() - size / 2, diagRect.center().y() - size / 2, size, size);

}
