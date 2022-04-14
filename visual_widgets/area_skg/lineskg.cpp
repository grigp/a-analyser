#include "lineskg.h"

#include "signalaccess.h"
#include "areaskgdefines.h"

#include <QPainter>
#include <QWidget>
#include <QDebug>

LineSKG::LineSKG(int diap, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_diap(diap)
{

}

QRectF LineSKG::boundingRect() const
{
    QPointF pos(-m_diap * m_prop - AreaSKGDefines::I_LABEL_SPACE, -m_diap * m_prop - AreaSKGDefines::I_LABEL_SPACE);
    QSizeF size((m_diap * m_prop + AreaSKGDefines::I_LABEL_SPACE) * 2, (m_diap * m_prop + AreaSKGDefines::I_LABEL_SPACE) * 2);
    return QRectF(pos, size);
}

void LineSKG::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(item);
    if (!m_signal)
        return;

    //! Параметры построения
    m_width = widget->size().width();
    m_height = widget->size().height();
    int minS = qMin(m_width, m_height);
    m_prop = static_cast<double>(minS / 2 - AreaSKGDefines::I_LABEL_SPACE) / static_cast<double>(m_diap);

    painter->save();

    //! Статокинезиграмма
    if (m_visible)
    {
        painter->setPen(QPen(m_colorSKG, 1));
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;

        int b = 0;
        if (m_begin != -1)
            b = m_begin;
        int e = m_signal->size();
        if (m_end != -1)
            e = m_end;

        for (int i = b; i < e; ++i)
        {
            if (m_isZeroing)
            {
                x1 = (m_signal->value(0, i) - m_offsX) * m_prop;
                y1 = - (m_signal->value(1, i) - m_offsY) * m_prop;
            }
            else
            {
                x1 = m_signal->value(0, i) * m_prop;
                y1 = - m_signal->value(1, i) * m_prop;
            }
            if (i > b)
                painter->drawLine(x1, y1, x2, y2);
            x2 = x1;
            y2 = y1;
        }
    }

    //! Эллипс
    painter->setPen(QPen(m_colorEllipse, 1));
    if (m_sizeA > 0 && m_sizeA < 5000
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
        painter->setPen(QPen(m_colorEllipse));
        while (psi < 2 * M_PI)
        {
            if (m_isZeroing)
            {
                x = lround((m_sizeA * cos(psi) * cos(-fi) + m_sizeB * sin(psi) * sin(-fi)) * m_prop);
                y = lround((-m_sizeA * cos(psi) * sin(-fi) + m_sizeB * sin(psi) * cos(-fi)) * m_prop);
            }
            else
            {
                x = lround((m_sizeA * cos(psi) * cos(-fi) + m_sizeB * sin(psi) * sin(-fi) + m_offsX) * m_prop);
                y = lround((-m_sizeA * cos(psi) * sin(-fi) + m_sizeB * sin(psi) * cos(-fi) - m_offsY) * m_prop);
            }

            if (!first)
                painter->drawLine(ox, oy, x, y);

            ox = x;
            oy = y;
            first = false;
            psi = psi + 2 * M_PI / 360;
        }

        //! Ось
        if (m_isZeroing)
        {
            x = lround((m_sizeA * 1.1 * cos(0) * cos(-fi) + m_sizeB * sin(0) * sin(-fi)) * m_prop);
            y = lround((-m_sizeA * 1.1 * cos(0) * sin(-fi) + m_sizeB * sin(0) * cos(-fi)) * m_prop);
            ox = lround((m_sizeA * 1.1 * cos(M_PI) * cos(-fi) + m_sizeB * sin(M_PI) * sin(-fi)) * m_prop);
            oy = lround((-m_sizeA * 1.1 * cos(M_PI) * sin(-fi) + m_sizeB * sin(M_PI) * cos(-fi)) * m_prop);
        }
        else
        {
            x = lround((m_sizeA * 1.1 * cos(0) * cos(-fi) + m_sizeB * sin(0) * sin(-fi) + m_offsX) * m_prop);
            y = lround((-m_sizeA * 1.1 * cos(0) * sin(-fi) + m_sizeB * sin(0) * cos(-fi) - m_offsY) * m_prop);
            ox = lround((m_sizeA * 1.1 * cos(M_PI) * cos(-fi) + m_sizeB * sin(M_PI) * sin(-fi) + m_offsX) * m_prop);
            oy = lround((-m_sizeA * 1.1 * cos(M_PI) * sin(-fi) + m_sizeB * sin(M_PI) * cos(-fi) - m_offsY) * m_prop);
        }
        painter->drawLine(ox, oy, x, y);

    }

    painter->restore();
}

void LineSKG::setDiap(int diap)
{
    m_diap = diap;
    updateItem();
}

void LineSKG::setSignal(SignalAccess *signal, const int begin, const int end)
{
    m_signal = signal;
    m_begin = begin;
    m_end = end;

    m_offsX = 0;
    m_offsY = 0;
    for (int i = 0; i < m_signal->size(); ++i)
    {
        m_offsX = m_offsX + m_signal->value(0, i);
        m_offsY = m_offsY + m_signal->value(1, i);
    }
    m_offsX = m_offsX / m_signal->size();
    m_offsY = m_offsY / m_signal->size();
}

void LineSKG::setSection(const int begin, const int end)
{
    m_begin = begin;
    m_end = end;
}

void LineSKG::setZeroing(const bool zeroing)
{
    m_isZeroing = zeroing;
    updateItem();
}

void LineSKG::setOffset(const double offsetX, const double offsetY)
{
    m_offsX = offsetX;
    m_offsY = offsetY;
    updateItem();
}

void LineSKG::setEllipse(const double sizeA, const double sizeB, const double angle)
{
    m_sizeA = sizeA;
    m_sizeB = sizeB;
    m_angle = angle;
    update(boundingRect());
}

void LineSKG::setVisible(const bool isVisible)
{
    m_visible = isVisible;
    update(boundingRect());
}

void LineSKG::updateItem()
{
    int minS = qMin(m_width, m_height);
    m_prop = static_cast<double>(minS / 2 - AreaSKGDefines::I_LABEL_SPACE) / static_cast<double>(m_diap);
    update(boundingRect());
}
