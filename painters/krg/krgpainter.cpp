#include "krgpainter.h"

#include "baseutils.h"

namespace  {
///< Отступы от краев виджета
static const int LeftSpace = 50;
static const int RightSpace = 10;
static const int TopSpace = 10;
static const int BottomSpace = 50;
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
        m_painter->drawLine(x, m_geometry.top() + m_geometry.height() - BottomSpace - d,
                            x, m_geometry.top() + m_geometry.height() - BottomSpace + d);
        m_painter->drawText(x - size.width() / 2, m_geometry.top() + m_geometry.height() - BottomSpace + 20, s);


        int y = static_cast<int>(m_geometry.top() + m_geometry.height() - BottomSpace - i * propY);
        m_painter->drawLine(m_geometry.left() + LeftSpace - d, y,
                            m_geometry.left() + LeftSpace + d, y);

        m_painter->translate(m_geometry.center().x(), m_geometry.center().y());
        m_painter->rotate(90);
        m_painter->translate(-m_geometry.center().x(), -m_geometry.center().y());

        m_painter->drawText(x - size.width() / 2, m_geometry.top() + m_geometry.height() - BottomSpace + 20, s);
//        m_painter->drawText(y, m_geometry.left() + LeftSpace - d, s);

        m_painter->translate(m_geometry.center().x(), m_geometry.center().y());
        m_painter->rotate(-90);
        m_painter->translate(-m_geometry.center().x(), -m_geometry.center().y());
    }

}

void KRGPainter::doUpdate()
{

}
