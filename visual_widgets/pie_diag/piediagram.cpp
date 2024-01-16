#include "piediagram.h"
#include "ui_piediagram.h"

#include <QPainter>
#include <QDebug>

#include "baseutils.h"

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
        QColor col = m_factors.at(i).color;
        painter.setBrush(QBrush(col, Qt::SolidPattern));
        painter.setPen(QPen(col, 1, Qt::SolidLine, Qt::FlatCap));

        int spanAngle = static_cast<int>(m_factors.at(i).percent / 100 * 5760);
        painter.drawPie(diagRect, startAngle, spanAngle);

        startAngle += spanAngle;
    }

    //! Внутренний круг
    painter.setBrush(QBrush(backColor, Qt::SolidPattern));
    painter.setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawEllipse(diagRect.center().x() - size / 2, diagRect.center().y() - size / 2, size, size);

    //! Метки
    startAngle = 90 * 16;
    for (int i = 0; i < m_factors.size(); ++i)
    {
        int spanAngle = static_cast<int>(m_factors.at(i).percent / 100 * 5760);

        QColor col = m_factors.at(i).color;
        auto midAngle = startAngle + spanAngle / 2;

        QColor colNeg = QColor(255 - col.red(), 255 - col.green(), 255 - col.blue());
        painter.setPen(QPen(colNeg, 1, Qt::SolidLine, Qt::FlatCap));

        QString text = "(" + QString::number(i + 1) + ")" + QString::number(static_cast<int>(m_factors.at(i).percent)) + "%";
        auto ts = BaseUtils::getTextSize(&painter, text);

        if(midAngle - 90 * 16 < 90 * 16)
        {
            double midAR = ((midAngle - (90 * 16)) / 16) * M_PI / 180;
            painter.drawText(diagRect.center().x() - static_cast<int>(3 * size / 4 * sin(midAR)) - ts.width() / 2,
                             diagRect.center().y() - static_cast<int>(3 * size / 4 * cos(midAR)),
                             text);
        }
        else
        if(midAngle - 90 * 16 < 180 * 16)
        {
            double midAR = ((midAngle - (180 * 16)) / 16) * M_PI / 180;
            painter.drawText(diagRect.center().x() - static_cast<int>(3 * size / 4 * cos(midAR)) - ts.width() / 2,
                             diagRect.center().y() + static_cast<int>(3 * size / 4 * sin(midAR)),
                             text);
        }
        else
        if(midAngle - 90 * 16 < 270 * 16)
        {
            double midAR = ((midAngle - (270 * 16)) / 16) * M_PI / 180;
            painter.drawText(diagRect.center().x() + static_cast<int>(3 * size / 4 * sin(midAR)) - ts.width() / 2,
                             diagRect.center().y() + static_cast<int>(3 * size / 4 * cos(midAR)),
                             text);
        }
        else
        if(midAngle - 90 * 16 < 360 * 16)
        {
            double midAR = ((midAngle - (360 * 16)) / 16) * M_PI / 180;
            painter.drawText(diagRect.center().x() + static_cast<int>(3 * size / 4 * cos(midAR)) - ts.width() / 2,
                             diagRect.center().y() - static_cast<int>(3 * size / 4 * sin(midAR)),
                             text);
        }

        startAngle += spanAngle;
    }

    painter.restore();
}
