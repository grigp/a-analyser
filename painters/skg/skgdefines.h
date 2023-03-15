#ifndef SKGDEFINES_H
#define SKGDEFINES_H

#include <QPainter>

namespace SKGDefines {

static const int I_LABEL_SPACE = 20;

/*!
 * \brief Структура, описывающая ломаную линию BrokenLine struct
 */
struct BrokenLine
{
    QPolygonF polygon;   ///< Полигон (список точек)
    QColor color;        ///< Цвет линий
    int width;           ///< Толщина линий
};

}

#endif // SKGDEFINES_H
