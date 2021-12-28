#include "triangledefines.h"

void TriangleDefines::Triangle::calculate()
{
    m_square = 0.5 * ((m_topCorner.x() - m_rightDownCorner.x()) * (m_leftDownCorner.y() - m_rightDownCorner.y()) -
                      (m_leftDownCorner.x() - m_rightDownCorner.x()) * (m_topCorner.y() - m_rightDownCorner.y()));
    m_mx = (m_topCorner.x() + m_leftDownCorner.x() + m_rightDownCorner.x()) / 3;
    m_my = (m_topCorner.y() + m_leftDownCorner.y() + m_rightDownCorner.y()) / 3;
}

void TriangleDefines::Triangle::setTimeFactors(const double time, const double speed)
{
    m_time = time; m_speed = speed;
}
