#include "dynamicdiagram.h"
#include "ui_dynamicdiagram.h"

#include "baseutils.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

namespace  {
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

QString DynamicDiagram::bottomText() const
{
    return m_bottomText;
}

void DynamicDiagram::setBottomText(const QString &text)
{
    m_bottomText = text;
    update();
}

QColor DynamicDiagram::backgroundColor() const
{
    return m_backgroundColor;
}

void DynamicDiagram::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
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

QColor DynamicDiagram::selectItemColor() const
{
    return m_selectItemColor;
}

void DynamicDiagram::setSelectItemColor(const QColor &color)
{
    m_selectItemColor = color;
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

void DynamicDiagram::setTitleHeight(const int height)
{
    m_titleHeight = height;
    update();
}

void DynamicDiagram::setAxisSpaceLeft(const int left)
{
    m_axisSpaceLeft = left;
    update();
}

void DynamicDiagram::setAxisSpaceBottom(const int bottom)
{
    m_axisSpaceBottom = bottom;
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
    m_title = "";
    update();
}

void DynamicDiagram::doSelectItem(const int idx)
{
    if (idx >= 0 && idx < m_items.size())
    {
        m_selectItem = idx;
        update();
    }
}

void DynamicDiagram::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);


    QPainter painter(this);
    painter.save();

    double min = 0;
    double max = 0;
    computeDiap(min, max);
    m_sizeH = (geometry().width() - 3 - 2 * m_axisSpaceLeft) / (m_items.size() + 1);
    double prop = 0;
    if (max > min)
        prop = (geometry().height() - m_axisSpaceBottom - m_titleHeight - 3) / (max - min);
    int zeroY = geometry().height() - m_axisSpaceBottom - static_cast<int>((0 - min) * prop);

    //! Фон
    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    //! Заголовок
    painter.setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawText(QRect(0, 0, geometry().width(), m_titleHeight - 5), m_title, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

    painter.drawText(QRect(0, geometry().height() - m_axisSpaceBottom + m_titleHeight, geometry().width(), m_titleHeight - 5),
                     m_bottomText, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

    //painter.drawRect(QRect(0, TitleHeight, geometry().width() - 3, geometry().height() - TitleHeight - 3));

    //! Оси
    painter.drawLine(m_axisSpaceLeft, geometry().height() - m_axisSpaceBottom, m_axisSpaceLeft, m_titleHeight);
    painter.drawLine(m_axisSpaceLeft, geometry().height() - m_axisSpaceBottom,
                     geometry().width() - m_axisSpaceLeft, geometry().height() - m_axisSpaceBottom);
    //! Линия 0
    if (min <= 0 && max >= 0)
        painter.drawLine(m_axisSpaceLeft, zeroY, geometry().width() - m_axisSpaceLeft, zeroY);

    //! Размер шрифта для отметок
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    //! Сетка по оси значений
    showValuesGrid(painter, min, max, prop);

    int ox = -1;
    int oy = -1;
    for (int i = 0; i < m_items.size(); ++i)
    {
        int xv = m_axisSpaceLeft + static_cast<int>((i + 1) * m_sizeH);
        int yv = geometry().height() - m_axisSpaceBottom - static_cast<int>((m_items.at(i)->value() - min) * prop);
        if (m_kind == KindBar)
        {
            int x = xv - static_cast<int>(m_sizeH / 10);
            int w = static_cast<int>(m_sizeH / 5);
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
            QColor barColor = m_diagColor;
            if (i == m_selectItem)
                barColor = m_selectItemColor;
            painter.setBrush(QBrush(barColor, Qt::SolidPattern));
            painter.setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
            painter.drawRect(x, y, w, h);
            if (m_volume == Volume3D)
            {
                auto dk = getDarkColor(barColor);
                painter.setBrush(QBrush(dk, Qt::SolidPattern));
                QPoint ptTop[4] = {
                    QPoint(x, y),
                    QPoint(x + 10, y - 10),
                    QPoint(x + w + 10, y - 10),
                    QPoint(x + w, y)
                };
                painter.drawPolygon(ptTop, 4);
                painter.setBrush(QBrush(getDarkColor(dk), Qt::SolidPattern));
                QPoint ptRight[4] = {
                    QPoint(x + w, y),
                    QPoint(x + w + 10, y - 10),
                    QPoint(x + w + 10, y + h - 10),
                    QPoint(x + w, y + h)
                };
                painter.drawPolygon(ptRight, 4);
            }
        }
        else
        if (m_kind == KindGraph)
        {
            if (i > 0)
            {
                if (m_volume == Volume2D)
                {
                    painter.setBrush(QBrush(m_diagColor, Qt::SolidPattern));
                    painter.setPen(QPen(m_diagColor, 4, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLine(ox, oy, xv, yv);
                }
                else
                if (m_volume == Volume3D)
                {
                    painter.setBrush(QBrush(m_diagColor, Qt::SolidPattern));
                    painter.setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
                    QPoint points[4] = {
                        QPoint(ox, oy),
                        QPoint(ox + 10, oy - 10),
                        QPoint(xv + 10, yv - 10),
                        QPoint(xv, yv)
                    };
                    painter.drawPolygon(points, 4);
                }
            }
            //! Вертикальный отсчет
            painter.setPen(QPen(m_titleColor, 1, Qt::DotLine, Qt::FlatCap));
            painter.drawLine(xv, geometry().height() - m_axisSpaceBottom,
                             xv, geometry().height() - m_axisSpaceBottom - static_cast<int>((max - min) * prop));

            if (i == m_selectItem)
            {
                if (m_volume == Volume2D)
                {
                    painter.setPen(QPen(m_selectItemColor, 3, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLine(xv, geometry().height() - m_axisSpaceBottom,
                                     xv, geometry().height() - static_cast<int>(m_axisSpaceBottom - (max - min) * prop));
                }
                else
                if (m_volume == Volume3D)
                {
                    painter.setBrush(QBrush(m_selectItemColor, Qt::SolidPattern));
                    painter.setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
                    QPoint points[4] = {
                        QPoint(xv, geometry().height() - m_axisSpaceBottom),
                        QPoint(xv, geometry().height() - m_axisSpaceBottom - static_cast<int>((max - min) * prop)),
                        QPoint(xv + 10, geometry().height() - m_axisSpaceBottom - static_cast<int>((max - min) * prop - 10)),
                        QPoint(xv + 10, geometry().height() - m_axisSpaceBottom - 10)
                    };
                    painter.drawPolygon(points, 4);
                }
            }

            ox = xv;
            oy = yv;
        }

        auto size = BaseUtils::getTextSize(&painter, m_items.at(i)->name());
        painter.setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
        painter.drawText(xv - size.width() / 2, geometry().height() - m_axisSpaceBottom + size.height(), m_items.at(i)->name());
    }

    painter.restore();
}

void DynamicDiagram::mousePressEvent(QMouseEvent *event)
{
    //! Выделение элемента
    int idx = static_cast<int>((event->x() + m_sizeH / 2 - m_axisSpaceLeft) / m_sizeH - 1);
    if (idx < 0)
        idx = 0;
    if (idx >= m_items.size())
        idx = m_items.size() - 1;
    m_selectItem = idx;
    emit selectItem(m_selectItem);
    update();

    QWidget::mousePressEvent(event);
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
        {
            min = 0;
            max = max * 1.1; // + 10%
        }
        else
        if (min < 0 && max < 0)
        {
            max = 0;
            min = min * 1.1; // + 10%
        }
    }
    else
    if (m_kind == KindGraph)
    {
        double max_ = max;
        double min_ = min;
        min = min - (max_ - min_) * 0.1;
        max = max + (max_ - min_) * 0.1;
    }
}

QColor DynamicDiagram::getDarkColor(const QColor &colBase) const
{
    auto red = colBase.red() / 2;
    auto green = colBase.green() / 2;
    auto blue = colBase.blue() / 2;
    return QColor(red, green, blue);
}

void DynamicDiagram::showValuesGrid(QPainter &painter,
                                    const double min, const double max,
                                    const double prop)
{
    double step = 1;
    if (max - min < 0.2)
        step = 0.01;
    else
    if (max - min < 2)
        step = 0.1;
    else
    if (max - min < 20)
        step = 1;
    else
    if (max - min < 200)
        step = 10;
    else
    if (max - min < 2000)
        step = 100;
    else
    if (max - min < 20000)
        step = 1000;
    else
    if (max - min < 100000)
        step = 5000;
    else
    if (max - min < 200000)
        step = 10000;
    else
    if (max - min < 1000000)
        step = 50000;

    int prec = 0;
    if (step < 1)
        prec = 1;
    if(step < 0.1)
        prec = 2;

    double cnt = min;
    while (cnt < max)
    {
        int y = geometry().height() - m_axisSpaceBottom - static_cast<int>((cnt - min) * prop);
        painter.setPen(QPen(m_titleColor, 1, Qt::DotLine, Qt::FlatCap));
        painter.drawLine(m_axisSpaceLeft, y, geometry().width() - m_axisSpaceLeft, y);

        QString lblMark = QString::number(cnt, 'f', prec);
        painter.drawText(QRect(0, y - 5, m_axisSpaceLeft, 10), lblMark, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

        cnt = cnt + step;
    }
}
