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
    QPointF topCorner;
    QPointF leftDownCorner;
    QPointF rightDownCorner;

    Triangle(QPointF top, QPointF leftDown, QPointF rightDown)
        : topCorner(top), leftDownCorner(leftDown), rightDownCorner(rightDown) {}
};

}

#endif // TRIANGLEDEFINES_H
