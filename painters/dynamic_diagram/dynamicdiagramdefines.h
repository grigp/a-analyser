#ifndef DYNAMICDIAGRAMDEFINES_H
#define DYNAMICDIAGRAMDEFINES_H

namespace DynamicDiagramDefines
{
///< Вид динамики
enum Kind
{
      KindGraph = 0   ///< График
    , KindBar         ///< Столбиковая диаграмма
};

///< Объем
enum Volume
{
      Volume2D = 0  ///< Двухмерная диаграмма
    , Volume3D      ///< Трехмерная диаграмма
};


}

#endif // DYNAMICDIAGRAMDEFINES_H
