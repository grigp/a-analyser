#include "ellipse.h"

#include <QPainter>
#include <QWidget>

static const int I_LABEL_SPACE = 20;

Ellipse::Ellipse(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{

}

QRectF Ellipse::boundingRect() const
{
    QPointF pos(-m_diap * m_prop - I_LABEL_SPACE, -m_diap * m_prop - I_LABEL_SPACE);
    QSizeF size((m_diap * m_prop + I_LABEL_SPACE) * 2, (m_diap * m_prop + I_LABEL_SPACE) * 2);
    return QRectF(pos, size);
}

void Ellipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    // Параметры построения
    m_width = widget->size().width();
    m_height = widget->size().height();
    int minS = qMin(m_width, m_height);
    m_prop = static_cast<double>(minS / 2 - I_LABEL_SPACE) / static_cast<double>(m_diap);
//    int left = -m_diap * m_prop;
//    int top = -m_diap * m_prop;
//    int right = m_diap * m_prop;
//    int bottom = m_diap * m_prop;

    painter->save();

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
        painter->setPen(QPen(Qt::darkBlue));
        while (psi < 2 * M_PI)
        {
            x = lround((m_sizeA * cos(psi) * cos(-fi) + m_sizeB * sin(psi) * sin(-fi) - m_offsX + m_centerX) * m_prop);
            y = lround((-m_sizeA * cos(psi) * sin(-fi) + m_sizeB * sin(psi) * cos(-fi) - m_offsY + m_centerY) * m_prop);
//        X := Round((FEllipseA*COS(Psi)*COS(-Fi) + FEllipseB*SIN(Psi)*SIN(-Fi) - FOffsetX + FStab.Middle[sciFrontal]) * PropX);
//        Y := Round((-FEllipseA*COS(Psi)*SIN(-Fi) + FEllipseB*SIN(Psi)*COS(-Fi) - FOffsetY + FStab.Middle[sciSagittal]) * PropX);

            if (!first)
                painter->drawLine(ox, oy, x, y);

            ox = x;
            oy = y;
            first = false;
            psi = psi + 2 * M_PI / 360;
        }

        //! Ось

    }

    painter->restore();
}

void Ellipse::setDiap(int diap)
{
    //! Если до первой прорисовки сделать изменение масштаба, появятся коллизии
    if (m_width > 0 && m_height > 0)
    {
        m_diap = diap;
        int minS = qMin(m_width, m_height);
        m_prop = static_cast<double>(minS / 2 - I_LABEL_SPACE) / static_cast<double>(m_diap);
        update(boundingRect());
    }
}

void Ellipse::setEllipse(const double centerX, const double centerY,
                         const double sizeA, const double sizeB, const double angle)
{
    m_centerX = centerX;
    m_centerY = centerY;
    m_sizeA = sizeA;
    m_sizeB = sizeB;
    m_angle = angle;
    update(boundingRect());
}
