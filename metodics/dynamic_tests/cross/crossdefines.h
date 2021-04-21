#ifndef CROSSDEFINES_H
#define CROSSDEFINES_H

#include <QMap>

namespace CrossDefines
{
/*!
 * \brief Выбрать направления ChangeStateMode enum
 */
enum ChangeStateMode
{
      csmReturn = 0     ///< Возврата в центр
    , csmFixedTime      ///< Заданное время
};

static QMap<QString, ChangeStateMode> ChangeStateModeValueIndex {
    std::pair<QString, ChangeStateMode> ("return", csmReturn)
  , std::pair<QString, ChangeStateMode> ("fixed_time", csmFixedTime)
};

static QMap<ChangeStateMode, QString> ChangeStateModeValueName {
    std::pair<ChangeStateMode, QString> (csmReturn, "return")
  , std::pair<ChangeStateMode, QString> (csmFixedTime, "fixed_time")
};

/*!
 * \brief Выбрать направления ChangeStateMode enum
 */
enum DirectionMode
{
      dmRandom = 0         ///< По случайному закону
    , dmClockwise          ///< По часовой стрелке
    , dmCounterClockwise   ///< Против часовой стрелке
};

static QMap<QString, DirectionMode> DirectionModeValueIndex {
    std::pair<QString, DirectionMode> ("return", dmRandom)
  , std::pair<QString, DirectionMode> ("clockwise", dmClockwise)
  , std::pair<QString, DirectionMode> ("counter_clockwise", dmCounterClockwise)
};

static QMap<DirectionMode, QString> DirectionModeValueName {
    std::pair<DirectionMode, QString> (dmRandom, "return")
  , std::pair<DirectionMode, QString> (dmClockwise, "clockwise")
  , std::pair<DirectionMode, QString> (dmCounterClockwise, "counter_clockwise")
};

}

#endif // CROSSDEFINES_H
