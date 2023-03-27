#include "crosspainter.h"

#include <QPalette>

CrossPainter::CrossPainter(QPainter *painter, QRect geometry)
    : m_painter(painter), m_geometry(geometry)
{

}

CrossPainter::CrossPainter()
{

}

CrossPainter::~CrossPainter()
{

}

void CrossPainter::setCanvas(QPainter *painter, QRect geometry, QWidget *wgt)
{
    m_painter = painter;
    m_geometry = geometry;
    m_widget = wgt;
}

void CrossPainter::setValueUp(const int value)
{
    m_valueUp = value;
    doUpdate();
}

void CrossPainter::setValueDown(const int value)
{
    m_valueDown = value;
    doUpdate();
}

void CrossPainter::setValueRight(const int value)
{
    m_valueRight = value;
    doUpdate();
}

void CrossPainter::setValueLeft(const int value)
{
    m_valueLeft = value;
    doUpdate();
}

void CrossPainter::doPaint(const double ratio)
{
    QColor backColor = Qt::white;
    if (m_widget)
        backColor = m_widget->palette().background().color();
    m_painter->setBrush(QBrush(backColor , Qt::SolidPattern));
    m_painter->setPen(QPen(backColor , 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(m_geometry);

    int mx = m_geometry.width() / 2;
    int my = m_geometry.height() / 2;

    auto font = m_painter->font();
    font.setPixelSize(22);
    font.setBold(true);
    m_painter->setFont(font);

    if (m_diap > 0)
    {
        m_painter->setBrush(QBrush(m_frontalColor, Qt::SolidPattern));
        m_painter->setPen(QPen(m_frontalColor, 1, Qt::SolidLine, Qt::FlatCap));
        int w = (mx - 10) * m_valueLeft / m_diap;
        m_painter->drawRect(mx - 10 - w, my - 10, w, 20);
        w = (mx - 10) * m_valueRight / m_diap;
        m_painter->drawRect(mx + 10, my - 10, w, 20);
        if (m_isShowValueLeft)
            m_painter->drawText(10, my + 35, QString::number(m_valueLeft));
        if (m_isShowValueRight)
            m_painter->drawText(m_geometry.width() - 50, my + 35, QString::number(m_valueRight));

        m_painter->setBrush(QBrush(m_sagittalColor, Qt::SolidPattern));
        m_painter->setPen(QPen(m_sagittalColor, 1, Qt::SolidLine, Qt::FlatCap));
        int h = (my - 10) * m_valueUp / m_diap;
        m_painter->drawRect(mx - 10, my - 10 - h, 20, h);
        h = (my - 10) * m_valueDown / m_diap;
        m_painter->drawRect(mx - 10, my + 10, 20, h);
        if (m_isShowValueUp)
            m_painter->drawText(mx + 15, 22, QString::number(m_valueUp));
        if (m_isShowValueDown)
            m_painter->drawText(mx + 15, m_geometry.height() - 10, QString::number(m_valueDown));
    }

    m_painter->setPen(QPen(m_frameColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->setBrush(QBrush(m_frameColor, Qt::NoBrush));
    m_painter->drawRect(0, my - 10, mx - 10, 20);
    m_painter->drawRect(mx + 10, my - 10, mx - 10 - 1, 20);
    m_painter->drawRect(mx - 10, 0, 20, my - 10);
    m_painter->drawRect(mx - 10, my + 10, 20, my - 10 - 1);
}

void CrossPainter::doUpdate()
{

}
