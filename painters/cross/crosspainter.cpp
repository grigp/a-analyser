#include "crosspainter.h"

#include <QPalette>
#include <QDebug>

#include "baseutils.h"

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
    m_painter->setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(m_geometry);

    int mx = m_geometry.center().x();
    int my = m_geometry.center().y();
    int thick = static_cast<int>(qMin(m_geometry.width() - m_geometry.left(),
                                      m_geometry.height() - m_geometry.top())
                                 * 0.06);  //! 6% от ширины

    m_painter->setFont(QFont("Arial", static_cast<int>(22 / (ratio / 2)), 0, false));
    QString sVL = QString::number(m_valueLeft);
    auto sizeVL = BaseUtils::getTextSize(m_painter, sVL);
    QString sVR = QString::number(m_valueRight);
    auto sizeVR = BaseUtils::getTextSize(m_painter, sVR);
    QString sVU = QString::number(m_valueUp);
    auto sizeVU = BaseUtils::getTextSize(m_painter, sVU);
    QString sVD = QString::number(m_valueDown);
//    auto sizeVD = BaseUtils::getTextSize(m_painter, sVD);

    if (m_diap > 0)
    {
        m_painter->setBrush(QBrush(m_frontalColor, Qt::SolidPattern));
        m_painter->setPen(QPen(m_frontalColor, 1, Qt::SolidLine, Qt::FlatCap));
        int w = (mx - thick/2 - m_geometry.left()) * m_valueLeft / m_diap;
        m_painter->drawRect(mx - thick/2 - w, my - thick/2, w, thick);
        w = (mx - thick/2 - m_geometry.left()) * m_valueRight / m_diap;
        m_painter->drawRect(mx + thick/2, my - thick/2, w, thick);
        if (m_isShowValueLeft)
            m_painter->drawText(m_geometry.left() + 10, my + thick / 2 + sizeVL.height() + 5, sVL);
        if (m_isShowValueRight)
            m_painter->drawText(m_geometry.left() + m_geometry.width() - sizeVR.width() - 5, my + thick / 2 + sizeVR.height() + 5, sVR);

        m_painter->setBrush(QBrush(m_sagittalColor, Qt::SolidPattern));
        m_painter->setPen(QPen(m_sagittalColor, 1, Qt::SolidLine, Qt::FlatCap));
        int h = (my - thick/2 - m_geometry.top()) * m_valueUp / m_diap;
        m_painter->drawRect(mx - thick/2, my - thick/2 - h, thick, h);
        h = (my - thick/2 - m_geometry.top()) * m_valueDown / m_diap;
        m_painter->drawRect(mx - thick/2, my + thick/2, thick, h);
        if (m_isShowValueUp)
            m_painter->drawText(mx + thick / 2 + 15, m_geometry.top() + sizeVU.height() + 5, sVU);
        if (m_isShowValueDown)
            m_painter->drawText(mx + thick / 2 + 15, m_geometry.top() + m_geometry.height() - 5, sVD);
    }

    m_painter->setPen(QPen(m_frameColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->setBrush(QBrush(m_frameColor, Qt::NoBrush));
    m_painter->drawRect(m_geometry.left(), my - thick/2, mx - thick/2 - m_geometry.left(), thick);
    m_painter->drawRect(mx + thick/2, my - thick/2, mx - thick/2 - m_geometry.left() - 1, thick);
    m_painter->drawRect(mx - thick/2, m_geometry.top(), thick, my - thick/2 - m_geometry.top());
    m_painter->drawRect(mx - thick/2, my + thick/2, thick, my - thick/2 - m_geometry.top() - 1);
}

void CrossPainter::doUpdate()
{

}
