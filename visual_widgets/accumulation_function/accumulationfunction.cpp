#include "accumulationfunction.h"
#include "ui_accumulationfunction.h"

#include <QPainter>
#include <QDebug>

#include "baseutils.h"
#include "vectorfactors.h"

namespace  {
///< Отступы от краев виджета
static const int LeftSpace = 50;
static const int RightSpace = 5;
static const int TopSpace = 20;
static const int BottomSpace = 40;
}

AccumulationFunction::AccumulationFunction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccumulationFunction)
{
    ui->setupUi(this);
}

AccumulationFunction::~AccumulationFunction()
{
    delete ui;
}

void AccumulationFunction::setDiap(const double min, const double max)
{
    m_min = min;
    m_max = max;
}

void AccumulationFunction::add(const double v)
{
    m_function << v;
}

void AccumulationFunction::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.save();

    //! Фон
    auto backColor = palette().background().color();
    painter.setBrush(QBrush(backColor, Qt::SolidPattern));
    painter.setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(0, 0, width(), height());

    //! Оси
    painter.setPen(QPen(m_axizColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawLine(LeftSpace, TopSpace, LeftSpace, height() - BottomSpace);
    painter.drawLine(LeftSpace, height() - BottomSpace, width() - RightSpace, height() - BottomSpace);

    //! Заголовок
    painter.setFont(QFont("Sans", 10, QFont::Bold, false));
    auto size = BaseUtils::getTextSize(&painter, m_title);
    painter.drawText(width() / 2 - size.width() / 2, 2 + size.height(), m_title);

    //! Подпись оси X
    painter.setFont(QFont("Sans", 10, 0, false));
    size = BaseUtils::getTextSize(&painter, m_xAxizName);
    painter.drawText(width() / 2 - size.width() / 2, height() - size.height() / 2 + 4, m_xAxizName);

    //! Подпись оси Y
    painter.setFont(QFont("Sans", 10, 0, false));
    painter.rotate(-90);
    size = BaseUtils::getTextSize(&painter, m_yAxizName);
    painter.drawText(- height() / 2 - size.width() / 2, size.height(), m_yAxizName);
    painter.rotate(90);

    //! Пропорция и шаг
    double prop = static_cast<double>(height() - TopSpace - BottomSpace) / (m_max - m_min);
    double step = static_cast<double>(width() - LeftSpace - RightSpace) / static_cast<double>(m_function.size());

    //! График
    int ox = 0;
    int oy = 0;
    for (int i = 0; i < m_function.size(); ++i)
    {
        int x = static_cast<int>(LeftSpace + i * step);
        int y = static_cast<int>(height() - BottomSpace - m_function.at(i) * prop);

        //! Точка и линия
        painter.setPen(QPen(m_lineColor, 2, Qt::SolidLine, Qt::FlatCap));
        if (i > 0)
            painter.drawLine(ox, oy, x, y);
        painter.drawEllipse(x - 2, y - 2, 4, 4);

        //! Сетка по х
        auto s = QString::number(i + 1);
        auto size = BaseUtils::getTextSize(&painter, s);
        if ((i + 1) % 10 == 0 || i == VectorFactorsDefines::DiapsCount)
        {
            painter.setPen(QPen(m_axizColor, 1, Qt::DashLine, Qt::FlatCap));
            painter.drawLine(x, TopSpace, x, height() - BottomSpace);
            painter.drawText(x - size.width() / 2, height() - BottomSpace + size.height(), s);
        }
        else
        if ((i + 1) % 5 == 0)
        {
            painter.setPen(QPen(m_axizColor, 1, Qt::DotLine, Qt::FlatCap));
            painter.drawLine(x, TopSpace, x, height() - BottomSpace);
            painter.drawText(x - size.width() / 2, height() - BottomSpace + size.height(), s);
        }

        ox = x;
        oy = y;
    }

    //! Сетка по Y
    for (int i = static_cast<int>(m_min); i <= m_max; ++i)
    {
        auto s = QString::number(i + 1);
        auto size = BaseUtils::getTextSize(&painter, s);
        int y = static_cast<int>(height() - BottomSpace - i * prop);
        if ((i + 1) % 10 == 0)
        {
            painter.setPen(QPen(m_axizColor, 1, Qt::DashLine, Qt::FlatCap));
            painter.drawLine(LeftSpace, y, width() - RightSpace, y);
            painter.drawText(LeftSpace - size.width() - 5, y + size.height() / 2 - 4, s);
        }
        else
        if ((i + 1) % 5 == 0)
        {
            painter.setPen(QPen(m_axizColor, 1, Qt::DotLine, Qt::FlatCap));
            painter.drawLine(LeftSpace, y, width() - RightSpace, y);
            painter.drawText(LeftSpace - size.width() - 5, y + size.height() / 2 - 4, s);
        }
    }

    painter.restore();
}
