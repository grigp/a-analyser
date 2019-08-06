#include "dynamicdiagram.h"
#include "ui_dynamicdiagram.h"

#include <QPainter>
#include <QDebug>

namespace  {
static const int TitleHeight = 25;
static const int AxisSpace = 50;

}

DynamicDiagram::DynamicDiagram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DynamicDiagram)
{
    ui->setupUi(this);
}

DynamicDiagram::~DynamicDiagram()
{
    delete ui;
}

DynamicDiagram::Kind DynamicDiagram::kind() const
{
    return m_kind;
}

void DynamicDiagram::setKind(const DynamicDiagram::Kind kind)
{
    m_kind = kind;
    update();
}

DynamicDiagram::Volume DynamicDiagram::volume() const
{
    return m_volume;
}

void DynamicDiagram::setVolume(const DynamicDiagram::Volume volume)
{
    m_volume = volume;
    update();
}

QString DynamicDiagram::title() const
{
    return m_title;
}

void DynamicDiagram::setTitle(const QString &title)
{
    m_title = title;
    update();
}

QColor DynamicDiagram::backgroundColor() const
{
    return m_backgroundColor;
}

void DynamicDiagram::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    update();
}

QColor DynamicDiagram::diagColor() const
{
    return m_diagColor;
}

void DynamicDiagram::setDiagColor(const QColor &color)
{
    m_diagColor = color;
    update();
}

QColor DynamicDiagram::titleColor() const
{
    return m_titleColor;
}

void DynamicDiagram::setTitleColor(const QColor &color)
{
    m_titleColor = color;
    update();
}

void DynamicDiagram::appendItem(DiagItem *item)
{
    m_items.append(item);
    update();
}

void DynamicDiagram::insertItem(const int idx, DiagItem *item)
{
    m_items.insert(idx, item);
    update();
}

double DynamicDiagram::value(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_items.size());
    return m_items.at(idx)->value();
}

QString DynamicDiagram::itemName(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_items.size());
    return m_items.at(idx)->name();
}

void DynamicDiagram::clear()
{
    m_items.clear();
}

void DynamicDiagram::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);


    QPainter painter(this);
    painter.save();

    double min = 0;
    double max = 0;
    computeDiap(min, max);
    double sizeH = (geometry().width() - 3 - 2 * AxisSpace) / (m_items.size() + 1);
    double prop = 0;
    if (max > min)
        prop = (geometry().height() - AxisSpace - TitleHeight - 3) / (max - min);
    int zeroY = geometry().height() - AxisSpace - (0 - min) * prop;

    //! Фон
    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    //! Заголовок
    painter.setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(QRect(0, 0, geometry().width(), TitleHeight - 5), m_title, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

    //painter.drawRect(QRect(0, TitleHeight, geometry().width() - 3, geometry().height() - TitleHeight - 3));

    //! Оси
    painter.drawLine(AxisSpace, geometry().height() - AxisSpace, AxisSpace, TitleHeight);
    painter.drawLine(AxisSpace, geometry().height() - AxisSpace,
                     geometry().width() - AxisSpace, geometry().height() - AxisSpace);
    //! Линия 0
    if (min <= 0 && max >= 0)
        painter.drawLine(AxisSpace, zeroY, geometry().width() - AxisSpace, zeroY);

    int ox = -1;
    int oy = -1;
    for (int i = 0; i < m_items.size(); ++i)
    {
        int xv = AxisSpace + (i + 1) * sizeH;
        int yv = geometry().height() - AxisSpace - (m_items.at(i)->value() - min) * prop;
        if (m_kind == KindBar)
        {
            int x = xv - sizeH / 10;
            int w = sizeH / 5;
            int y = zeroY;
            int h = 0;
            if (m_items.at(i)->value() > 0)
            {
                y = yv;
                h = zeroY - y;
            }
            else
            {
                y = zeroY;
                h = yv - y;
            }
            painter.setBrush(QBrush(m_diagColor, Qt::SolidPattern));
            painter.setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
            painter.drawRect(x, y, w, h);
        }
        else
        if (m_kind == KindGraph)
        {
            painter.setBrush(QBrush(m_diagColor, Qt::SolidPattern));
            painter.setPen(QPen(m_diagColor, 4, Qt::SolidLine, Qt::FlatCap));
            if (i > 0)
                painter.drawLine(ox, oy, xv, yv);
            //! Ветрикальный отстчет
            painter.setPen(QPen(m_titleColor, 1, Qt::DotLine, Qt::FlatCap));
            painter.drawLine(xv, geometry().height() - AxisSpace,
                             xv, geometry().height() - AxisSpace - (max - min) * prop);
            ox = xv;
            oy = yv;
        }
        QFontMetrics fm(painter.font());
        int fw = fm.width(m_items.at(i)->name());
        int fh = fm.height();
        painter.drawText(xv - fw / 2, geometry().height() - AxisSpace + fh, m_items.at(i)->name());
    }

    painter.restore();
}

void DynamicDiagram::computeDiap(double &min, double &max)
{
    if (m_items.size() == 0)
    {
        min = 0;
        max = 0;
        return;
    }
    min = INT_MAX;
    max = - INT_MAX;
    foreach (auto* item, m_items)
    {
        if (item->value() < min)
            min = item->value();
        if (item->value() > max)
            max = item->value();
    }
    //! Столбиковые диаграммы начинаются от 0
    if (m_kind == KindBar)
    {
        if (min > 0 && max > 0)
            min = 0;
        else
        if (min < 0 && max < 0)
            max = 0;
    }

}
