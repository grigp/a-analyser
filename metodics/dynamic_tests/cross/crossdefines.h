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
 * \brief Перечень этапов Stage enum
 */
enum Stage
{
      stgNo = 0
    , stgGo
    , stgWait
};

//! Значение порога отклонения
static const int DeviationThresholdValue = 38;

}

#endif // CROSSDEFINES_H
