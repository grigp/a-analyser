#include "krgpainter.h"

#include <QDebug>

#include "baseutils.h"
#include "ritmogram.h"

namespace  {
///< Отступы от краев виджета
static const int LeftSpace = 50;
static const int RightSpace = 10;
static const int TopSpace = 10;
static const int BottomSpace = 50;

/*!
 * \brief Границы зоны The Bounds struct
 */
struct Bounds
{
    int lo {0};
    int hi {0};
    QColor color {Qt::white};
    Bounds(int l, int h, QColor c) : lo(l), hi(h), color(c) {}
};
/*!
 * \brief Список зон с границами и цветами
 */
QList<Bounds> NormBounds = QList<Bounds>() << Bounds(0, 50, QColor(255, 120, 120))
                                           << Bounds(50, 60, QColor(220, 220, 150))
                                           << Bounds(60, 90, QColor(100, 220, 100))
                                           << Bounds(90, 120, QColor(220, 220, 150))
                                           << Bounds(120, 200, QColor(255, 120, 120));
}

KRGPainter::KRGPainter(QPainter *painter, QRect geometry)
    : m_painter(painter), m_geometry(geometry)
{

}

KRGPainter::KRGPainter()
{

}

KRGPainter::~KRGPainter()
{

}

void KRGPainter::setCanvas(QPainter *painter, QRect geometry, QWidget *wgt)
{
    m_painter = painter;
    m_geometry = geometry;
    m_widget = wgt;
}

int KRGPainter::leftSpace() const
{
    return LeftSpace;
}

int KRGPainter::rightSpace() const
{
    return RightSpace;
}

int KRGPainter::topSpace() const
{
    return TopSpace;
}

int KRGPainter::bottomSpace() const
{
    return BottomSpace;
}

void KRGPainter::doPaint(const double ratio)
{
    m_painter->save();

    QColor backColor = Qt::white;
    if (m_widget)
        backColor = m_widget->palette().background().color();

    //! Фон
    m_painter->setBrush(QBrush(backColor, Qt::SolidPattern));
    m_painter->setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(m_geometry.left(), m_geometry.top(), m_geometry.width(), m_geometry.height());

    m_painter->setPen(QPen(m_colorAxis, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawLine(m_geometry.left() + LeftSpace, m_geometry.top() + TopSpace,
                        m_geometry.left() + LeftSpace, m_geometry.top() + m_geometry.height() - BottomSpace);
    m_painter->drawLine(m_geometry.left() + LeftSpace, m_geometry.top() + m_geometry.height() - BottomSpace,
                        m_geometry.left() + m_geometry.width() - RightSpace, m_geometry.top() + m_geometry.height() - BottomSpace);

    double propX = static_cast<double>(m_geometry.width() - m_geometry.left() - LeftSpace - RightSpace) / 200.0;
    double propY = static_cast<double>(m_geometry.height() - m_geometry.top() - TopSpace - BottomSpace) / 200.0;

    m_painter->setFont(QFont("Arial", static_cast<int>(8 / (ratio / 4)), 0, false));

    //! Подписи к осям
    auto size = BaseUtils::getTextSize(m_painter, m_nameHorizontal);
    m_painter->drawText(m_geometry.left() + m_geometry.width() / 2 - size.width() / 2,
                        m_geometry.top() + m_geometry.height() - size.height(),
                        m_nameHorizontal);

    size = BaseUtils::getTextSize(m_painter, m_nameVertical);
    int x = m_geometry.left() + 5;
    int y = m_geometry.top() + m_geometry.height() / 2 - size.height() / 2;

    m_painter->translate(x, y);
    m_painter->rotate(-90);
    m_painter->translate(-x, -y);

    m_painter->drawText(x, y + 10, m_nameVertical);

    m_painter->translate(x, y);
    m_painter->rotate(90);
    m_painter->translate(-x, -y);

    //! Зоны КРГ
    foreach (auto bound, NormBounds)
    {
        m_painter->setBrush(QBrush(bound.color, Qt::SolidPattern));
        m_painter->setPen(QPen(bound.color, 1, Qt::SolidLine, Qt::FlatCap));

        int xLo = static_cast<int>(m_geometry.left() + LeftSpace + bound.lo * propX);
        int xHi = static_cast<int>(m_geometry.left() + LeftSpace + bound.hi * propX);
        int yLo = static_cast<int>(m_geometry.top() + m_geometry.height() - BottomSpace - bound.lo * propY);
        int yHi = static_cast<int>(m_geometry.top() + m_geometry.height() - BottomSpace - bound.hi * propY);

        m_painter->drawRect(xLo, yLo - (yLo - yHi), (xHi - xLo), (yLo - yHi));
    }

    //! Сетка
    for (int i = 0; i <= 200; i+=5)
    {
        QString s = QString::number(i);
        auto size = BaseUtils::getTextSize(m_painter, s);

        int d = 2;
        if (i % 50 == 0)
            d = 6;
        else
        if (i % 10 == 0)
            d = 4;
        int x = static_cast<int>(m_geometry.left() + LeftSpace + i * propX);
        m_painter->setPen(QPen(m_colorAxis, 1, Qt::SolidLine, Qt::FlatCap));
        m_painter->drawLine(x, m_geometry.top() + m_geometry.height() - BottomSpace - d,
                            x, m_geometry.top() + m_geometry.height() - BottomSpace + d);
        m_painter->drawText(x - size.width() / 2, m_geometry.top() + m_geometry.height() - BottomSpace + 20, s);


        int y = static_cast<int>(m_geometry.top() + m_geometry.height() - BottomSpace - i * propY);
        m_painter->drawLine(m_geometry.left() + LeftSpace - d, y,
                            m_geometry.left() + LeftSpace + d, y);
        m_painter->drawText(m_geometry.left() + LeftSpace - 10 - size.width(), y + size.height() / 3, s);

        if (i % 10 == 0)
        {
            m_painter->setPen(QPen(m_colorAxis, 1, Qt::DotLine, Qt::FlatCap));
            m_painter->drawLine(x, m_geometry.top() + m_geometry.height() - BottomSpace - d,
                                x, m_geometry.top() +TopSpace);
            m_painter->drawLine(m_geometry.left() + LeftSpace + d, y,
                                m_geometry.left() + m_geometry.width() - RightSpace, y);
        }

//        Попытки написать текст повернутый
//        m_painter->translate(m_geometry.center().x(), m_geometry.center().y());
//        m_painter->rotate(-90);
//        m_painter->translate(-m_geometry.center().x(), -m_geometry.center().y());

//        m_painter->drawText(x - size.width() / 2, m_geometry.top() + TopSpace - 20, s);
//////        m_painter->drawText(x - size.width() / 2, m_geometry.top() + m_geometry.height() - BottomSpace + 20, s);
//////        m_painter->drawText(y, m_geometry.left() + LeftSpace - d, s);

//        m_painter->translate(m_geometry.center().x(), m_geometry.center().y());
//        m_painter->rotate(90);
//        m_painter->translate(-m_geometry.center().x(), -m_geometry.center().y());
    }

    if (m_signal)
    {
        for (int i = 0; i < m_signal->size() - 1; ++i)
        {
            double r1 = m_signal->value(i);
            double r2 = m_signal->value(i + 1);

            int x = static_cast<int>(m_geometry.left() + LeftSpace + r2 * propX);
            int y = static_cast<int>(m_geometry.top() + m_geometry.height() - BottomSpace - r1 * propY);

            m_painter->setBrush(QBrush(m_colorKRG, Qt::SolidPattern));
            m_painter->setPen(QPen(m_colorKRGBound, 1, Qt::SolidLine, Qt::FlatCap));
            m_painter->drawEllipse(x - 1, y - 1, 3, 3);
        }

    }

    m_painter->restore();
}

void KRGPainter::appendSignal(Ritmogram *signal)
{
    m_signal = signal;
    doUpdate();
}

void KRGPainter::doUpdate()
{

}
