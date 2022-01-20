#ifndef AREASKGDEFINES_H
#define AREASKGDEFINES_H

#include <QPainter>

namespace AreaSKGDefines {

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

#endif // AREASKGDEFINES_H
