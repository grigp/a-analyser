#include "skgpainter.h"

#include <QDebug>

SKGPainter::SKGPainter(QPainter* painter, QRect geometry)
    : QObject(), m_painter(painter), m_geometry(geometry)
{

}

int SKGPainter::diap() const
{

}

void SKGPainter::setDiap(const int diap)
{

}

void SKGPainter::addMarker()
{

}

void SKGPainter::setMarker(const double x, const double y)
{

}

void SKGPainter::setMarkerColor(const QColor colorBackground, const QColor colorBorder)
{

}

void SKGPainter::showTrace(const bool trace)
{

}

void SKGPainter::setSignal(SignalAccess *signal, const int num, const int begin, const int end)
{

}

void SKGPainter::setSection(const int begin, const int end, const int num)
{

}

void SKGPainter::setVisibleMarker(const bool visibleMarker)
{

}

void SKGPainter::setZeroing(const bool zeroing)
{

}

void SKGPainter::setOffset(const double offsetX, const double offsetY, const int num)
{

}

void SKGPainter::setEllipse(const double sizeA, const double sizeB, const double angle)
{

}

void SKGPainter::setColorSKG(const QColor &color, const int num)
{

}

QColor SKGPainter::colorSKG(const int num) const
{

}

void SKGPainter::setColorEllipse(const QColor &color)
{

}

QColor SKGPainter::colorEllipse() const
{

}

QColor SKGPainter::colorMarker() const
{

}

void SKGPainter::setColorMarker(const QColor &color)
{

}

QColor SKGPainter::colorPlatforms() const
{

}

void SKGPainter::setColorPlatforms(const QColor &color)
{

}

void SKGPainter::addTarget(const double x, const double y, const QColor colorBackground, const QColor colorBorder)
{

}

void SKGPainter::setTarget(const double x, const double y, const int idx)
{

}

void SKGPainter::clearTargets()
{

}

int SKGPainter::addBrokenLine(SKGDefines::BrokenLine &bl)
{

}

bool SKGPainter::deleteBrokenLine(const int idx)
{

}

void SKGPainter::setVisibleSKG(const bool isVisible, const int num)
{

}

void SKGPainter::addPlatform(QRect platform)
{

}

void SKGPainter::doPaint(const double ratio)
{
//    int space = static_cast<int>(SKGDefines::I_LABEL_SPACE * ratio);
    m_space = static_cast<int>(SKGDefines::I_LABEL_SPACE);

    //! Параметры построения
    m_width = m_geometry.width();
    m_height = m_geometry.height();
    int minS = qMin(m_width, m_height);
    m_prop = static_cast<double>(minS / 2 - m_space) / static_cast<double>(m_diap);

    m_midX = m_geometry.center().x();
    m_midY = m_geometry.center().y();
    m_left = m_midX - static_cast<int>(m_diap * m_prop);
    m_top = m_midY - static_cast<int>(m_diap * m_prop);
    m_right = m_midX + static_cast<int>(m_diap * m_prop);
    m_bottom = m_midY + static_cast<int>(m_diap * m_prop);
    m_ratio = ratio;

    m_painter->save();

    drawGrid();

    m_painter->restore();
}

void SKGPainter::setAreaSKG()
{

}

QColor SKGPainter::getFrameColor(const QColor color) const
{

}

void SKGPainter::drawGrid()
{
    //! Оси и метки 0
    m_painter->setPen(QPen(Qt::black, 1));
    m_painter->drawRect(m_left, m_top, m_right - m_left, m_bottom - m_top);
    m_painter->drawLine(m_midX, m_top, m_midX, m_bottom);
    m_painter->drawLine(m_left, m_midY, m_right, m_midY);
    m_painter->setFont(QFont("Arial", static_cast<int>(8 / (m_ratio / 2)), 0, false));
    m_painter->drawText(QRectF(m_midX-15, m_top - m_space - m_ratio, 30, m_space), Qt::AlignCenter, "0");
    m_painter->drawText(QRectF(m_midX-15, m_bottom + 2 * m_ratio, 30, m_space), Qt::AlignCenter, "0");
    m_painter->translate(m_midX, m_midY);
    m_painter->rotate(90);
    m_painter->translate(-m_midX, -m_midY);
    m_painter->drawText(QRectF(m_midX-15, m_top - m_space - m_ratio, 30, m_space), Qt::AlignCenter, "0");
    m_painter->drawText(QRectF(m_midX-15, m_bottom + 2 * m_ratio, 30, m_space), Qt::AlignCenter, "0");
    m_painter->translate(m_midX, m_midY);
    m_painter->rotate(-90);
    m_painter->translate(-m_midX, -m_midY);

    //! Сетка
    for (int i = 1; i <= m_diap; ++i)
    {
        if (m_diap >= 30)
        {
            if (i % 50 == 0)
            {
                m_painter->setPen(QPen(Qt::darkGray, 1, Qt::DashLine));
                drawPositionGrid(i, true);
            }
            else
            if (i % 10 == 0)
            {
                m_painter->setPen(QPen(Qt::darkGray, 1, Qt::DotLine));
                drawPositionGrid(i, m_prop * 10 > 20);
            }
        }
        else
        {
            if (i % 5 == 0)
            {
                m_painter->setPen(QPen(Qt::darkGray, 1, Qt::DashLine));
                drawPositionGrid(i, true);
            }
            else
            {
                m_painter->setPen(QPen(Qt::darkGray, 1, Qt::DotLine));
                drawPositionGrid(i, m_prop > 20);
            }
        }
    }
}

void SKGPainter::drawPositionGrid(int posGrid, bool isLabels)
{
    //! Линии
    m_painter->drawLine(static_cast<int>(m_midX - posGrid * m_prop), m_top, static_cast<int>(m_midX - posGrid * m_prop), m_bottom);
    m_painter->drawLine(static_cast<int>(m_midX + posGrid * m_prop), m_top, static_cast<int>(m_midX + posGrid * m_prop), m_bottom);
    m_painter->drawLine(m_left, static_cast<int>(m_midY - posGrid * m_prop), m_right, static_cast<int>(m_midY - posGrid * m_prop));
    m_painter->drawLine(m_left, static_cast<int>(m_midY + posGrid * m_prop), m_right, static_cast<int>(m_midY + posGrid * m_prop));

    m_painter->setPen(Qt::black);

    if (isLabels)
    {
        //! Текст горизонтальный
        m_painter->drawText(QRectF(m_midX - posGrid * m_prop - 10 * m_ratio, m_top - SKGDefines::I_LABEL_SPACE - m_ratio,
                                   20 * m_ratio, SKGDefines::I_LABEL_SPACE),
                            Qt::AlignCenter, QString::number(-posGrid));
        m_painter->drawText(QRectF(m_midX - posGrid * m_prop - 10 * m_ratio, m_bottom + 2 * m_ratio,
                                   20 * m_ratio, SKGDefines::I_LABEL_SPACE),
                            Qt::AlignCenter, QString::number(-posGrid));
        m_painter->drawText(QRectF(m_midX + posGrid * m_prop - 10 * m_ratio, m_top - SKGDefines::I_LABEL_SPACE - m_ratio,
                                   20 * m_ratio, SKGDefines::I_LABEL_SPACE),
                            Qt::AlignCenter, QString::number(posGrid));
        m_painter->drawText(QRectF(m_midX + posGrid * m_prop - 10 * m_ratio, m_bottom + 2 * m_ratio,
                                   20 * m_ratio, SKGDefines::I_LABEL_SPACE),
                            Qt::AlignCenter, QString::number(posGrid));

        //! Текст вертикальный
        m_painter->translate(m_midX, m_midY);
        m_painter->rotate(-90);
        m_painter->translate(-m_midX, -m_midY);
        m_painter->drawText(QRectF(m_midX - posGrid * m_prop - 10 * m_ratio, m_top - SKGDefines::I_LABEL_SPACE - m_ratio,
                                   20 * m_ratio, SKGDefines::I_LABEL_SPACE),
                            Qt::AlignCenter, QString::number(-posGrid));
        m_painter->drawText(QRectF(m_midX + posGrid * m_prop - 10 * m_ratio, m_top - SKGDefines::I_LABEL_SPACE - m_ratio,
                                   20 * m_ratio, SKGDefines::I_LABEL_SPACE),
                            Qt::AlignCenter, QString::number(posGrid));
        m_painter->translate(m_midX, m_midY);
        m_painter->rotate(180);
        m_painter->translate(-m_midX, -m_midY);
        m_painter->drawText(QRectF(m_midX - posGrid * m_prop - 10 * m_ratio, m_top - SKGDefines::I_LABEL_SPACE - m_ratio,
                                   20 * m_ratio, SKGDefines::I_LABEL_SPACE),
                            Qt::AlignCenter, QString::number(posGrid));
        m_painter->drawText(QRectF(m_midX + posGrid * m_prop - 10 * m_ratio, m_top - SKGDefines::I_LABEL_SPACE - m_ratio,
                                   20 * m_ratio, SKGDefines::I_LABEL_SPACE),
                            Qt::AlignCenter, QString::number(-posGrid));
        m_painter->translate(m_midX, m_midY);
        m_painter->rotate(-90);
        m_painter->translate(-m_midX, -m_midY);
    }
}
