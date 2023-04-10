#include "dynamicdiagrampainter.h"

#include "baseutils.h"

#include <QDebug>

DynamicDiagramPainter::DynamicDiagramPainter(QPainter *painter, QRect geometry)
    : m_painter(painter), m_geometry(geometry)
{

}

DynamicDiagramPainter::DynamicDiagramPainter()
{

}

DynamicDiagramPainter::~DynamicDiagramPainter()
{

}

void DynamicDiagramPainter::setCanvas(QPainter *painter, QRect geometry, QWidget *wgt)
{
    m_painter = painter;
    m_geometry = geometry;
    m_widget = wgt;
}

DynamicDiagramPainter::Kind DynamicDiagramPainter::kind() const
{
    return m_kind;
}

void DynamicDiagramPainter::setKind(const DynamicDiagramPainter::Kind kind)
{
    m_kind = kind;
    doUpdate();
}

DynamicDiagramPainter::Volume DynamicDiagramPainter::volume() const
{
    return m_volume;
}

void DynamicDiagramPainter::setVolume(const DynamicDiagramPainter::Volume volume)
{
    m_volume = volume;
    doUpdate();
}

QString DynamicDiagramPainter::title() const
{
    return m_title;
}

void DynamicDiagramPainter::setTitle(const QString &title)
{
    m_title = title;
    doUpdate();
}

QString DynamicDiagramPainter::bottomText() const
{
    return m_bottomText;
}

void DynamicDiagramPainter::setBottomText(const QString &text)
{
    m_bottomText = text;
    doUpdate();
}

QColor DynamicDiagramPainter::diagColor() const
{
    return m_diagColor;
}

void DynamicDiagramPainter::setDiagColor(const QColor &color)
{
    m_diagColor = color;
    doUpdate();
}

QColor DynamicDiagramPainter::selectItemColor() const
{
    return m_selectItemColor;
}

void DynamicDiagramPainter::setSelectItemColor(const QColor &color)
{
    m_selectItemColor = color;
    doUpdate();
}

QColor DynamicDiagramPainter::titleColor() const
{
    return m_titleColor;
}

void DynamicDiagramPainter::setTitleColor(const QColor &color)
{
    m_titleColor = color;
    doUpdate();
}

void DynamicDiagramPainter::setTitleHeight(const int height)
{
    m_titleHeight = height;
    doUpdate();
}

void DynamicDiagramPainter::setAxisSpaceLeft(const int left)
{
    m_axisSpaceLeft = left;
    doUpdate();
}

void DynamicDiagramPainter::setAxisSpaceBottom(const int bottom)
{
    m_axisSpaceBottom = bottom;
    doUpdate();
}

void DynamicDiagramPainter::appendItem(DiagItem *item)
{
    m_items.append(item);
    doUpdate();
}

void DynamicDiagramPainter::insertItem(const int idx, DiagItem *item)
{
    m_items.insert(idx, item);
    doUpdate();
}

double DynamicDiagramPainter::value(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_items.size());
    return m_items.at(idx)->value();
}

QString DynamicDiagramPainter::itemName(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_items.size());
    return m_items.at(idx)->name();
}

void DynamicDiagramPainter::setDiap(const double min, const double max)
{
    m_minimum = min;
    m_maximum = max;
    doUpdate();
}

void DynamicDiagramPainter::clear()
{
    m_items.clear();
    m_title = "";
    doUpdate();
}

void DynamicDiagramPainter::doSelectItem(const int idx)
{
    if (idx >= 0 && idx < m_items.size())
    {
        m_selectItem = idx;
        doUpdate();
    }
}

void DynamicDiagramPainter::doPaint(const double ratio)
{
    m_painter->save();

    double min = 0;
    double max = 0;
    if (fabs(m_minimum) < INT_MAX / 2 && fabs(m_maximum) < INT_MAX / 2)
    {
        min = m_minimum;
        max = m_maximum;
    }
    else
        computeDiap(min, max);
    m_sizeH = (m_geometry.width() - m_geometry.left() - 3 - 2 * m_axisSpaceLeft) / (m_items.size() + 1);
    double prop = 0;
    if (max > min)
        prop = (m_geometry.height() - m_axisSpaceBottom - m_titleHeight - 3) / (max - min);
    int zeroY = m_geometry.top() + m_geometry.height() - m_axisSpaceBottom - static_cast<int>((0 - min) * prop);

    //! Фон
    QColor backColor = Qt::white;
    if (m_widget)
        backColor = m_widget->palette().background().color();
    m_painter->setBrush(QBrush(backColor , Qt::SolidPattern));
    m_painter->setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    //m_painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(m_geometry);

    double r = ratio;

    //! Заголовок
    m_painter->setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
    //! Размер шрифта для отметок
    m_painter->setFont(QFont("Arial", static_cast<int>(10 / r), QFont::Bold, false));
    m_painter->drawText(QRect(m_geometry.left(), m_geometry.top(),
                              static_cast<int>(m_geometry.width()), static_cast<int>(m_titleHeight * ratio - 5)),
                        m_title, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

    m_painter->setFont(QFont("Arial", static_cast<int>(10 / r), 0, false));
    m_painter->drawText(QRect(m_geometry.left(), m_geometry.top() + m_geometry.height() - m_axisSpaceBottom + m_titleHeight,
                              m_geometry.width(), m_titleHeight - 5),
                        m_bottomText, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

    //painter.drawRect(QRect(0, TitleHeight, geometry().width() - 3, geometry().height() - TitleHeight - 3));

    //! Оси
    m_painter->drawLine(m_geometry.left() + m_axisSpaceLeft, m_geometry.top() + m_geometry.height() - m_axisSpaceBottom,
                        m_geometry.left() + m_axisSpaceLeft, m_geometry.top() + m_titleHeight);
    m_painter->drawLine(m_geometry.left() + m_axisSpaceLeft, m_geometry.top() + m_geometry.height() - m_axisSpaceBottom,
                        m_geometry.left() + m_geometry.width() - m_axisSpaceLeft, m_geometry.top() + m_geometry.height() - m_axisSpaceBottom);
    //! Линия 0
    if (min <= 0 && max >= 0)
        m_painter->drawLine(m_geometry.left() + m_axisSpaceLeft, zeroY, m_geometry.width() - m_axisSpaceLeft, zeroY);

    //! Размер шрифта для отметок
    m_painter->setFont(QFont("Arial", static_cast<int>(10 / r), 0, false));

    //! Сетка по оси значений
    showValuesGrid(*m_painter, min, max, prop, r);

    int ox = -1;
    int oy = -1;
    for (int i = 0; i < m_items.size(); ++i)
    {
        int xv = m_geometry.left() + m_axisSpaceLeft + static_cast<int>((i + 1) * m_sizeH);
        int yv = m_geometry.top() + m_geometry.height() - m_axisSpaceBottom - static_cast<int>((m_items.at(i)->value() - min) * prop);
        if (m_kind == KindBar)
        {
            int x = xv - static_cast<int>(m_sizeH / 10);
            int w = static_cast<int>(m_sizeH / m_sizeDivider);  //5);
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
            m_painter->setBrush(QBrush(barColor, Qt::SolidPattern));
            m_painter->setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
            m_painter->drawRect(x, y, w, h);
            if (m_volume == Volume3D)
            {
                auto dk = getDarkColor(barColor);
                m_painter->setBrush(QBrush(dk, Qt::SolidPattern));
                QPoint ptTop[4] = {
                    QPoint(x, y),
                    QPoint(x + 10, y - 10),
                    QPoint(x + w + 10, y - 10),
                    QPoint(x + w, y)
                };
                m_painter->drawPolygon(ptTop, 4);
                m_painter->setBrush(QBrush(getDarkColor(dk), Qt::SolidPattern));
                QPoint ptRight[4] = {
                    QPoint(x + w, y),
                    QPoint(x + w + 10, y - 10),
                    QPoint(x + w + 10, y + h - 10),
                    QPoint(x + w, y + h)
                };
                m_painter->drawPolygon(ptRight, 4);
            }
        }
        else
        if (m_kind == KindGraph)
        {
            if (i > 0)
            {
                if (m_volume == Volume2D)
                {
                    m_painter->setBrush(QBrush(m_diagColor, Qt::SolidPattern));
                    m_painter->setPen(QPen(m_diagColor, 4, Qt::SolidLine, Qt::FlatCap));
                    m_painter->drawLine(ox, oy, xv, yv);
                }
                else
                if (m_volume == Volume3D)
                {
                    m_painter->setBrush(QBrush(m_diagColor, Qt::SolidPattern));
                    m_painter->setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
                    QPoint points[4] = {
                        QPoint(ox, oy),
                        QPoint(ox + 10, oy - 10),
                        QPoint(xv + 10, yv - 10),
                        QPoint(xv, yv)
                    };
                    m_painter->drawPolygon(points, 4);
                }
            }
            //! Вертикальный отсчет
            m_painter->setPen(QPen(m_titleColor, 1, Qt::DotLine, Qt::FlatCap));
            m_painter->drawLine(xv, m_geometry.height() - m_axisSpaceBottom,
                                xv, m_geometry.height() - m_axisSpaceBottom - static_cast<int>((max - min) * prop));

            if (i == m_selectItem)
            {
                if (m_volume == Volume2D)
                {
                    m_painter->setPen(QPen(m_selectItemColor, 3, Qt::SolidLine, Qt::FlatCap));
                    m_painter->drawLine(xv, m_geometry.height() - m_axisSpaceBottom,
                                        xv, m_geometry.height() - static_cast<int>(m_axisSpaceBottom - (max - min) * prop));
                }
                else
                if (m_volume == Volume3D)
                {
                    m_painter->setBrush(QBrush(m_selectItemColor, Qt::SolidPattern));
                    m_painter->setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
                    QPoint points[4] = {
                        QPoint(xv, m_geometry.height() - m_axisSpaceBottom),
                        QPoint(xv, m_geometry.height() - m_axisSpaceBottom - static_cast<int>((max - min) * prop)),
                        QPoint(xv + 10, m_geometry.height() - m_axisSpaceBottom - static_cast<int>((max - min) * prop - 10)),
                        QPoint(xv + 10, m_geometry.height() - m_axisSpaceBottom - 10)
                    };
                    m_painter->drawPolygon(points, 4);
                }
            }

            ox = xv;
            oy = yv;
        }

        auto so = BaseUtils::getTextSize(m_painter, "9999");
        auto size = BaseUtils::getTextSize(m_painter, m_items.at(i)->name());
        m_painter->setPen(QPen(m_titleColor, 1, Qt::SolidLine, Qt::FlatCap));
        if (so.width() < m_sizeH)
            m_painter->drawText(xv - size.width() / 2, m_geometry.height() - m_axisSpaceBottom + size.height(), m_items.at(i)->name());
        else
        {
            if ((i+1) % 5 == 0)
                m_painter->drawText(xv - size.width() / 2, m_geometry.height() - m_axisSpaceBottom + size.height(), m_items.at(i)->name());
        }
    }

    m_painter->restore();
}

void DynamicDiagramPainter::doUpdate()
{

}

int DynamicDiagramPainter::doSelectItemByX(const int x)
{
    //! Выделение элемента
    int idx = static_cast<int>((x + m_sizeH / 2 - m_axisSpaceLeft) / m_sizeH - 1);
    if (idx < 0)
        idx = 0;
    if (idx >= m_items.size())
        idx = m_items.size() - 1;
    m_selectItem = idx;
    return m_selectItem;
}

void DynamicDiagramPainter::computeDiap(double &min, double &max)
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

QColor DynamicDiagramPainter::getDarkColor(const QColor &colBase) const
{
    auto red = colBase.red() / 2;
    auto green = colBase.green() / 2;
    auto blue = colBase.blue() / 2;
    return QColor(red, green, blue);
}

void DynamicDiagramPainter::showValuesGrid(QPainter &painter, const double min, const double max, const double prop, const double ratio)
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
    m_painter->setFont(QFont("Arial", static_cast<int>(8 / ratio), 0, false));
    while (cnt < max)
    {
        int y = m_geometry.top() + m_geometry.height() - m_axisSpaceBottom - static_cast<int>((cnt - min) * prop);
        painter.setPen(QPen(m_titleColor, 1, Qt::DotLine, Qt::FlatCap));
        painter.drawLine(m_geometry.left() + m_axisSpaceLeft, y, m_geometry.left() + m_geometry.width() - m_axisSpaceLeft, y);

        QString lblMark = QString::number(cnt, 'f', prec);
        auto sz = BaseUtils::getTextSize(m_painter, lblMark);
        QRect rect(static_cast<int>(m_geometry.left() + m_axisSpaceLeft - sz.width() - 10 * ratio), y - 5,
                   static_cast<int>(m_axisSpaceLeft * ratio), static_cast<int>(10 * ratio * 1.5));
        painter.drawText(rect, lblMark, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

        cnt = cnt + step;
    }
}
