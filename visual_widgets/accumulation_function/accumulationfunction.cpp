#include "accumulationfunction.h"
#include "ui_accumulationfunction.h"

#include <QPainter>
#include <QDebug>

#include "baseutils.h"

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
    painter.drawText(width() / 2 - size.width() / 2, height() - size.height() / 2, m_xAxizName);

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
    for (int i = 1; i < m_function.size(); ++i)
    {
        int x1 = static_cast<int>(LeftSpace + (i - 1) * step);
        int x2 = static_cast<int>(LeftSpace + i * step);

        int y1 = static_cast<int>(height() - BottomSpace - m_function.at(i - 1) * prop);
        int y2 = static_cast<int>(height() - BottomSpace - m_function.at(i) * prop);

        painter.setPen(QPen(m_lineColor, 2, Qt::SolidLine, Qt::FlatCap));
        painter.drawLine(x1, y1, x2, y2);

        auto s = QString::number(i);
        auto size = BaseUtils::getTextSize(&painter, s);
        if (i % 10 == 0)
        {
            painter.setPen(QPen(m_axizColor, 1, Qt::DashLine, Qt::FlatCap));
            painter.drawLine(x1, TopSpace, x1, height() - BottomSpace);
            painter.drawText(x1 - size.width() / 2, height() - BottomSpace + size.height(), s);
        }
        else
        if (i % 5 == 0)
        {
            painter.setPen(QPen(m_axizColor, 1, Qt::DotLine, Qt::FlatCap));
            painter.drawLine(x1, TopSpace, x1, height() - BottomSpace);
            painter.drawText(x1 - size.width() / 2, height() - BottomSpace + size.height(), s);
        }
    }

    painter.restore();
}
