#include "triangledefines.h"

void TriangleDefines::Triangle::calculate()
{
    m_square = 0.5 * ((m_topCorner.x() - m_rightDownCorner.x()) * (m_leftDownCorner.y() - m_rightDownCorner.y()) -
                      (m_leftDownCorner.x() - m_rightDownCorner.x()) * (m_topCorner.y() - m_rightDownCorner.y()));
    m_mx = (m_topCorner.x() + m_leftDownCorner.x() + m_rightDownCorner.x()) / 3;
    m_my = (m_topCorner.y() + m_leftDownCorner.y() + m_rightDownCorner.y()) / 3;

    //! Угол наклона треугольника
    double medX = (m_leftDownCorner.x() + m_rightDownCorner.x()) / 2;
    double medY = (m_leftDownCorner.y() + m_rightDownCorner.y()) / 2;
    double r = sqrt(pow(medX - m_topCorner.x(), 2) + pow(medY - m_topCorner.y(), 2));
    m_angle = asin((m_topCorner.x() - medX) / r) / M_PI * 180;
}

void TriangleDefines::Triangle::setTimeFactors(const double time, const double speed)
{
    m_time = time; m_speed = speed;
}

QPointF TriangleDefines::Triangle::center(QPointF top, QPointF right, QPointF left)
{
    QPointF retval;
    retval.setX((top.x() + left.x() + right.x()) / 3);
    retval.setY((top.y() + left.y() + right.y()) / 3);
    return retval;
}
