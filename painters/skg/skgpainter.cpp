#include "skgpainter.h"

#include "aanalyserapplication.h"
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

void SKGPainter::setCanvas(QPainter *painter, QRect geometry, QWidget *wgt)
{
    m_painter = painter;
    m_geometry = geometry;
    m_widget = wgt;
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
    m_isVisibleMarker = true;
}

void SKGPainter::setMarker(const double x, const double y)
{
    m_mx = x;
    m_my = y;
    if (m_isShowTrace)
        m_trace << QPointF(x, y);
    doUpdate();
}

void SKGPainter::setMarkerColor(const QColor colorBackground, const QColor colorBorder)
{
    m_markerColor = colorBackground;
    m_markerBorderColor = colorBorder;
}

void SKGPainter::setMarkerSize(const int size)
{
    m_mSize = size;
}

void SKGPainter::showTrace(const bool trace)
{
    Q_UNUSED(trace);
    m_isShowTrace = trace;
}

void SKGPainter::clearTrace()
{
    m_trace.clear();
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
    if (num >= 0 && num < m_signals.size())
    {
        auto sd = m_signals.at(num);
        sd.begin = begin;
        sd.end = end;
        m_signals.replace(num, sd);
        doUpdate();
    }
}

void SKGPainter::setVisibleMarker(const bool visibleMarker)
{
    m_isVisibleMarker = visibleMarker;
}

void SKGPainter::setZeroing(const bool zeroing)
{
    m_isZeroing = zeroing;
    doUpdate();
}

void SKGPainter::setOffset(const double offsetX, const double offsetY, const int num)
{
    if (num >= 0 && num < m_signals.size())
    {
        auto sd = m_signals.at(num);
        sd.offsX = offsetX;
        sd.offsY = offsetY;
        m_signals.replace(num, sd);
    }
}

void SKGPainter::setEllipse(const double sizeA, const double sizeB, const double angle)
{
    m_sizeA = sizeA;
    m_sizeB = sizeB;
    m_angle = angle;
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
    m_ellipseColor = color;
}

QColor SKGPainter::colorEllipse() const
{
    return m_ellipseColor;
}

QColor SKGPainter::colorMarker() const
{
    return m_markerColor;
}

void SKGPainter::setColorMarker(const QColor &color)
{
    m_markerColor = color;
}

QColor SKGPainter::colorPlatforms() const
{
    return m_platformsColor;
}

void SKGPainter::setColorPlatforms(const QColor &color)
{
   m_platformsColor = color;
}

void SKGPainter::addTarget(const double x, const double y, const QColor colorBackground, const QColor colorBorder, const bool visible)
{
    m_targets << TargetInfo(QSize(10, 10), QPointF(x, y), colorBackground, colorBorder, visible);
}

void SKGPainter::setTarget(const double x, const double y, const int idx)
{
    if (idx >= 0 && idx < m_targets.size())
    {
        auto ti = m_targets.at(idx);
        ti.pos = QPointF(x, y);
        m_targets.replace(idx, ti);
    }
}

void SKGPainter::clearTargets()
{
    m_targets.clear();
}

int SKGPainter::addBrokenLine(SKGDefines::BrokenLine &bl)
{
    m_brokenLines << bl;
    return  m_brokenLines.size() - 1;
}

bool SKGPainter::deleteBrokenLine(const int idx)
{
    if (idx >= 0 && idx < m_brokenLines.size())
    {
        m_brokenLines.removeAt(idx);
        return true;
    }
    return false;
}

void SKGPainter::setVisibleSKG(const bool isVisible, const int num)
{
    if (num >= 0 && num < m_signals.size())
    {
        auto sd = m_signals.at(num);
        sd.visible = isVisible;
        m_signals.replace(num, sd);
        doUpdate();
    }
}

void SKGPainter::addPlatform(QRect platform)
{
    m_platforms << platform;
}

void SKGPainter::doPaint(const double ratio)
{
    if (!m_painter || (m_geometry == QRect(0, 0, 0, 0)))
        return;

//    int space = static_cast<int>(SKGDefines::I_LABEL_SPACE * ratio);
    //! Диапазон между границами и зоной диаграммы для вывода отсчетов
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

    //! Пошаговое рисование
    drawPlatforms(ratio);
    drawGrid(ratio);
    drawSKG();
    drawBrokenLines(ratio);
    drawTargets();
    drawTrace();
    drawMarker();
    drawTitle();
}

void SKGPainter::doUpdate()
{

}

void SKGPainter::drawGrid(const double ratio)
{
    m_painter->save();
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
                drawPositionGrid(i, m_prop / (ratio/2) * 10 > 20);
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
                drawPositionGrid(i, m_prop / (ratio/2) > 20);
            }
        }
    }
    m_painter->restore();
}

void SKGPainter::drawPositionGrid(int posGrid, bool isLabels)
{
    m_painter->save();
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
    m_painter->restore();
}

void SKGPainter::drawSKG()
{
    m_painter->save();
    //! По сигналу
    for (int chan = 0; chan < m_signals.size(); ++chan)
    {
        //! Только, если сигнал видим
        if (m_signals.at(chan).visible)
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

    //! Эллипс. Рисуется, если заданы его параметры и виден сам сигнал
    if (m_signals.size() > 0 && m_signals.at(0).visible
         && m_sizeA > 0 && m_sizeA < 5000
         && m_sizeB > 0 && m_sizeB < 5000
         && m_angle >= -360 && m_angle <= 360)
    {
        double fi = - m_angle * M_PI / 180 - M_PI / 2;
        double psi = 0;
        int ox = 0;
        int oy = 0;
        int x = 0;
        int y = 0;
        bool first = true;
        m_painter->setPen(QPen(m_ellipseColor, 1));
        while (psi < 2 * M_PI)
        {
            if (m_isZeroing)
            {
                x = lround(m_midX + (m_sizeA * cos(psi) * cos(-fi) + m_sizeB * sin(psi) * sin(-fi)) * m_prop);
                y = lround(m_midY + (-m_sizeA * cos(psi) * sin(-fi) + m_sizeB * sin(psi) * cos(-fi)) * m_prop);
            }
            else
            {
                x = lround(m_midX + (m_sizeA * cos(psi) * cos(-fi) + m_sizeB * sin(psi) * sin(-fi) + m_signals.at(0).offsX) * m_prop);
                y = lround(m_midY + (-m_sizeA * cos(psi) * sin(-fi) + m_sizeB * sin(psi) * cos(-fi) - m_signals.at(0).offsY) * m_prop);
            }

            if (!first)
                m_painter->drawLine(ox, oy, x, y);

            ox = x;
            oy = y;
            first = false;
            psi = psi + 2 * M_PI / 360;
        }

        //! Ось
        if (m_isZeroing)
        {
            x = lround(m_midX + (m_sizeA * 1.1 * cos(0) * cos(-fi) + m_sizeB * sin(0) * sin(-fi)) * m_prop);
            y = lround(m_midY + (-m_sizeA * 1.1 * cos(0) * sin(-fi) + m_sizeB * sin(0) * cos(-fi)) * m_prop);
            ox = lround(m_midX + (m_sizeA * 1.1 * cos(M_PI) * cos(-fi) + m_sizeB * sin(M_PI) * sin(-fi)) * m_prop);
            oy = lround(m_midY + (-m_sizeA * 1.1 * cos(M_PI) * sin(-fi) + m_sizeB * sin(M_PI) * cos(-fi)) * m_prop);
        }
        else
        {
            x = lround(m_midX + (m_sizeA * 1.1 * cos(0) * cos(-fi) + m_sizeB * sin(0) * sin(-fi) + m_signals.at(0).offsX) * m_prop);
            y = lround(m_midY + (-m_sizeA * 1.1 * cos(0) * sin(-fi) + m_sizeB * sin(0) * cos(-fi) - m_signals.at(0).offsY) * m_prop);
            ox = lround(m_midX + (m_sizeA * 1.1 * cos(M_PI) * cos(-fi) + m_sizeB * sin(M_PI) * sin(-fi) + m_signals.at(0).offsX) * m_prop);
            oy = lround(m_midY + (-m_sizeA * 1.1 * cos(M_PI) * sin(-fi) + m_sizeB * sin(M_PI) * cos(-fi) - m_signals.at(0).offsY) * m_prop);
        }
        m_painter->drawLine(ox, oy, x, y);
    }

    m_painter->restore();
}

void SKGPainter::drawPlatforms(const double ratio)
{
//TODO: настраивать цвет платформ на печати в зависимости от стиля главного окна
//    qDebug() << static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->styleSheet();
//    static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->palette().color()

    m_painter->save();
    m_painter->setBrush(QBrush(m_platformsColor, Qt::SolidPattern));
    QColor frameColor = QColor(m_platformsColor.red() / 8, m_platformsColor.green() / 8, m_platformsColor.blue() / 8);
    m_painter->setPen(QPen(frameColor, 2, Qt::SolidLine, Qt::FlatCap));
    m_painter->setFont(QFont("Sans", static_cast<int>(20 / (ratio/2)), QFont::Bold, false));

    for (int i = 0; i < m_platforms.size(); ++i)
    {
        QRect r = QRect(static_cast<int>(m_midX + m_platforms.at(i).x() * m_prop),
                        static_cast<int>(m_midY - m_platforms.at(i).y() * m_prop),
                        static_cast<int>(m_platforms.at(i).width() * m_prop),
                        static_cast<int>(m_platforms.at(i).height() * m_prop));
        m_painter->drawRect(r);
        m_painter->drawText(r, QString::number(i + 1));
    }
    m_painter->restore();
}

void SKGPainter::drawBrokenLines(const double ratio)
{
    m_painter->save();
    //! По ломаным, их может быть несколько
    foreach (auto bLine, m_brokenLines)
    {
        m_painter->setPen(QPen(bLine.color, bLine.width * (ratio / 2)));

        //! По точкам ломаной
        QPolygonF plgn;
        foreach (auto point, bLine.polygon)
        {
            double x = m_midX + point.x() * m_prop;
            double y = m_midY - point.y() * m_prop;
            plgn << QPointF(x, y);
        }
        double x = m_midX + bLine.polygon.at(0).x() * m_prop;
        double y = m_midY - bLine.polygon.at(0).y() * m_prop;
        plgn << QPointF(x, y);

        m_painter->drawPolyline(plgn);
    }
    m_painter->restore();
}

void SKGPainter::drawTrace()
{
    if (m_trace.size() > 1)
    {
        double ox = 0;
        double oy = 0;
        for (int i = 0; i < m_trace.size(); ++i)
        {
            double x = m_midX + m_trace.at(i).x() * m_prop;
            double y = m_midY - m_trace.at(i).y() * m_prop;

            if (i > 0)
            {
                m_painter->save();
                m_painter->setPen(QPen(m_traceColor, 1, Qt::SolidLine, Qt::FlatCap));
                m_painter->drawLine(static_cast<int>(ox), static_cast<int>(oy), static_cast<int>(x), static_cast<int>(y));
                m_painter->restore();
            }

            ox = x;
            oy = y;
        }
    }
}

void SKGPainter::drawMarker()
{
    if (m_isVisibleMarker)
    {
        double x = m_midX + m_mx * m_prop;
        double y = m_midY - m_my * m_prop;

        m_painter->save();
        m_painter->setBrush(QBrush(m_markerColor, Qt::SolidPattern));
        m_painter->setPen(QPen(m_markerBorderColor, 1, Qt::SolidLine, Qt::FlatCap));
        m_painter->drawRect(static_cast<int>(x - m_mSize / 2), static_cast<int>(y - m_mSize / 2), m_mSize, m_mSize);
        m_painter->restore();
    }
}

void SKGPainter::drawTargets()
{
    m_painter->save();
    foreach (auto ti, m_targets)
    {
        if (ti.visible)
        {
            double x = m_midX + ti.pos.x() * m_prop;
            double y = m_midY - ti.pos.y() * m_prop;
            int w = ti.size.width();
            int h = ti.size.height();

            m_painter->setBrush(QBrush(ti.colorBackground, Qt::SolidPattern));
            m_painter->setPen(QPen(ti.colorBorder, 1, Qt::SolidLine, Qt::FlatCap));
            m_painter->drawRect(static_cast<int>(x - w / 2), static_cast<int>(y - h / 2), w, h);
        }
    }
    m_painter->restore();
}

void SKGPainter::drawTitle()
{
    if (m_title != "")
    {
        m_painter->save();

        QColor backColor = Qt::white;
        if (m_widget)
            backColor = m_widget->palette().background().color();
        m_painter->setBrush(QBrush(backColor, Qt::SolidPattern));
        m_painter->setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
        m_painter->drawRect(m_geometry.left(), m_geometry.top(), m_geometry.width(), 20);

        m_painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
        m_painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        m_painter->drawText(m_geometry.left(), m_geometry.top(), m_geometry.width(), 20, Qt::AlignHCenter | Qt::AlignVCenter, m_title);

        m_painter->restore();
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
