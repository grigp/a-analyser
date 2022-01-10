#ifndef TRIANGLEDEFINES_H
#define TRIANGLEDEFINES_H

#include <QPointF>

namespace TriangleDefines
{

/*!
 * \brief Этапы теста "Треугольник" Stage enum
 */
enum Stage
{
      stgWaiting = 0  ///< Ожидание
    , stgTraining     ///< Обучение
    , stgAnalysis     ///< Тренинг
};

/*!
 * \brief Структура данных о треугольнике Triangle struct
 */
struct Triangle
{
    Triangle()
    {m_topCorner = QPointF(0, 0); m_leftDownCorner = QPointF(0, 0); m_rightDownCorner = QPointF(0, 0);}
    Triangle(QPointF top, QPointF leftDown, QPointF rightDown)
        : m_topCorner(top), m_leftDownCorner(leftDown), m_rightDownCorner(rightDown) {calculate();}

    QPointF topCorner() const {return m_topCorner;}
    void setTopCorner(const QPointF corner) {m_topCorner = corner;}
    QPointF leftDownCorner() const {return m_leftDownCorner;}
    void setLeftDownCorner(const QPointF corner) {m_leftDownCorner = corner;}
    QPointF rightDownCorner() const {return m_rightDownCorner;}
    void setRightDownCorner(const QPointF corner) {m_rightDownCorner = corner;}

    double time() const {return  m_time;}
    double square() const {return m_square;}
    double speed() const {return m_speed;}
    double mx() const {return m_mx;}
    double my() const {return m_my;}

    void calculate();

    void setTimeFactors(const double time, const double speed);

    static QPointF center(QPointF top, QPointF right, QPointF left);

private:
    QPointF m_topCorner;
    QPointF m_leftDownCorner;
    QPointF m_rightDownCorner;

    double m_time {0};
    double m_square {0};
    double m_speed {0};
    double m_mx {0};
    double m_my {0};

};

}

#endif // TRIANGLEDEFINES_H
