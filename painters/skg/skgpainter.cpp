#include "skgpainter.h"

#include "signalaccess.h"
#include <QDebug>

SKGPainter::SKGPainter(QPainter* painter, QRect geometry)
    : m_painter(painter), m_geometry(geometry)
{

}

SKGPainter::SKGPainter()
{

}

SKGPainter::~SKGPainter()
{

}

void SKGPainter::setCanvas(QPainter *painter, QRect geometry)
{
    m_painter = painter;
    m_geometry = geometry;
}

int SKGPainter::diap() const
{
    return m_diap;
}

void SKGPainter::setDiap(const int diap)
{
    m_diap = diap;
    doUpdate();
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
    Q_UNUSED(begin);
    Q_UNUSED(end);
    if (num == m_signals.size())
        m_signals.append(SignalData(signal, Qt::darkCyan, begin, end));
    else
    if (num >= 0 && num < m_signals.size())
    {
        auto sd = m_signals.at(num);
        sd.assignSignal(signal);
        m_signals.replace(num, sd);
    }
}

void SKGPainter::setSection(const int begin, const int end, const int num)
{

}

void SKGPainter::setVisibleMarker(const bool visibleMarker)
{

}

void SKGPainter::setZeroing(const bool zeroing)
{
    m_isZeroing = zeroing;
    doUpdate();
}

void SKGPainter::setOffset(const double offsetX, const double offsetY, const int num)
{

}

void SKGPainter::setEllipse(const double sizeA, const double sizeB, const double angle)
{

}

void SKGPainter::setColorSKG(const QColor &color, const int num)
{
    if (num >= 0 && num < m_signals.size())
    {
        auto sd = m_signals.at(num);
        sd.color = color;
        m_signals.replace(num, sd);
    }
}

QColor SKGPainter::colorSKG(const int num) const
{
    if (num >= 0 && num < m_signals.size())
        return m_signals.at(num).color;
    return Qt::darkCyan;
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
//    qDebug() << m_painter << m_geometry << m_diap;
    if (!m_painter || (m_geometry == QRect(0, 0, 0, 0)))
        return;

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
    drawSKG();

    m_painter->restore();
}

void SKGPainter::doUpdate()
{

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

void SKGPainter::drawSKG()
{
    for (int chan = 0; chan < m_signals.size(); ++chan)
    {
        m_painter->setPen(QPen(m_signals.at(chan).color, 1));
        auto signal = m_signals.at(chan).signal;

        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;

        int b = 0;
        if (m_signals.at(chan).begin != -1)
            b = m_signals.at(chan).begin;
        int e = signal->size();
        if (m_signals.at(chan).end != -1)
            e = m_signals.at(chan).end;

        for (int i = b; i < e; ++i)
        {
            if (m_isZeroing)
            {
                x1 = static_cast<int>(m_midX + (signal->value(0, i) - m_signals.at(chan).offsX) * m_prop);
                y1 = static_cast<int>(m_midY - (signal->value(1, i) - m_signals.at(chan).offsY) * m_prop);
            }
            else
            {
                x1 = static_cast<int>(m_midX + m_signals.at(chan).signal->value(0, i) * m_prop);
                y1 = static_cast<int>(m_midY - m_signals.at(chan).signal->value(1, i) * m_prop);
            }
            if (i > b)
                m_painter->drawLine(x1, y1, x2, y2);
            x2 = x1;
            y2 = y1;
        }
    }
}

SKGPainter::SignalData::SignalData(SignalAccess *sig, const QColor col, const int b, const int e)
    : signal(sig), color(col), begin(b), end(e)
{
    assignSignal();  //! Рассчитать параметры сигнала
}

void SKGPainter::SignalData::assignSignal(SignalAccess *sig)
{
    if (sig != nullptr)
        signal = sig;
    offsX = 0;
    offsY = 0;
    for (int i = 0; i < signal->size(); ++i)
    {
        offsX = offsX + signal->value(0, i);
        offsY = offsY + signal->value(1, i);
    }
    offsX = offsX / signal->size();
    offsY = offsY / signal->size();
}
