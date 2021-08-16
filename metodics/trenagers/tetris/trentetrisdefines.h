#ifndef TRENTETRISDEFINES_H
#define TRENTETRISDEFINES_H

#include <QObject>
#include <QMap>

namespace TrenTetrisDefines
{

/*!
 * \brief Режим перемещения MovingMode enum
 */
enum MovingMode
{
      mmTake = 0     ///< Захват и спуск
    , mmAuto         ///< Автоматический спуск
};

static QMap<QString, MovingMode> MovingModeValueIndex {
    std::pair<QString, MovingMode> ("take", mmTake)
  , std::pair<QString, MovingMode> ("auto", mmAuto)
};

static QMap<MovingMode, QString> MovingModeValueName {
    std::pair<MovingMode, QString> (mmTake, "take")
  , std::pair<MovingMode, QString> (mmAuto, "auto")
};


/*!
 * \brief Сложность Complexity enum
 */
enum ComplexityMode
{
      cmFigures = 0      ///< Фигуры
    , cmCubes            ///< Одиночные кубики
};

static QMap<QString, ComplexityMode> ComplexityModeValueIndex {
    std::pair<QString, ComplexityMode> ("figures", cmFigures)
  , std::pair<QString, ComplexityMode> ("cubes", cmCubes)
};

static QMap<ComplexityMode, QString> ComplexityValueName {
    std::pair<ComplexityMode, QString> (cmFigures, "figures")
  , std::pair<ComplexityMode, QString> (cmCubes, "cubes")
};


/*!
 * \brief Режим удаления DeletingMode enum
 */
enum DeletingMode
{
      dmRows  = 0     ///< Строки
    , dmColored       ///< Кубиков одного цвета, расположенных рядом
};

static QMap<QString, DeletingMode> DeletingModeValueIndex {
    std::pair<QString, DeletingMode> ("rows", dmRows)
  , std::pair<QString, DeletingMode> ("colored", dmColored)
};

static QMap<DeletingMode, QString> DeletingValueName {
    std::pair<DeletingMode, QString> (dmRows, "rows")
  , std::pair<DeletingMode, QString> (dmColored, "colored")
};


}

#endif // TRENTETRISDEFINES_H
