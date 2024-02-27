#ifndef TRENTAKEPUTDEFINES_H
#define TRENTAKEPUTDEFINES_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QSizeF>
#include <QDebug>

#include "graphiccommon.h"

namespace TrenTakePutDefines
{

/*!
 * \brief Этапы игры GameStage enum
 */
enum GameStage
{
      gsTake = 0     ///< Поиск захвата
    , gsTakeProcess  ///< Процесс захвата
    , gsPut          ///< Поиск укладки
    , gsPutProcess   ///< Процесс укладки
};

/*!
 * \brief Типы позиции зоны PositionKind enum
 */
enum PositionKind
{
      pkRandom  ///< Случайная
    , pkFixed   ///< Фиксированная
};

/*!
 * \brief Порядок заполнения и захвата элементов
 */
enum TakeOrder
{
      toEnabledPrimary  ///< Сначала допустимые, потом остальные
                        ///<  1.1. Проходим по всем элементам enabled == true и расставляем их на зону по случайному закону
                        ///<  1.2. Проходим по всем элементам enabled == false и расставляем их по случайному закону на свободные зоны
                        ///<  1.3. Повторяем 1.2. пока есть пустые зоны
    , toAllByOrder      ///< Все по порядку
                        ///<  Если кол-во зон равно кол-ву элементов
                        ///<  Проходим по всем элементам. Для каждого элемента по случайному закону назначаем пустую зону.
};

/*!
 * \brief Режим прохождения этапов StageMode enum
 */
enum StageMode
{
      smTakePut       ///< Этап для одного элемента (взял - положил)
    , smAllElements   ///< Этап для всех элементов (все enabled элементы надо положить в свои зоны)
};

/*!
 * \brief Режим дополнений в маркере TargetAdvMode enum
 */
enum TargetAdvMode
{
      tamNone = 0         ///< Нет
    , tamTraceOnTarget    ///< Трассы, когда маркер на цели
};


/*!
 * \brief Структура информации о зоне захвата или укладки GameZoneInfo struct
 */
struct GameZoneInfo
{
public:
    PositionKind posKind {pkFixed};
    int x_min {0};                         ///< Диапазон значений позиции для posKind == pkRandom
    int x_max {0};
    int y_min {0};
    int y_max {0};
    int x {0};                             ///< Позиция для posKind == pkFixed
    int y {0};
    int width {50};                        ///< Размеры
    int height {50};
    int position {0};                      ///< Для стиля picture_split код позиции. Первая цифра - номер столбца, вторые две - номер строки
                                           ///< Теоретически 100 столбцов и 100 строк pos / 100 - столбец, pos % 100 - строка

    GraphicCommon::GameElement* element {nullptr};        ///< Назначенный для зоны графический элемент

    GameZoneInfo() {}

    void setElement(GraphicCommon::GameElement* elem) {element = elem;}
    void clearElement() {element = nullptr;}
};

}

#endif // TRENTAKEPUTDEFINES_H
