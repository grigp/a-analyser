#ifndef STEPOFFSETDEFINES_H
#define STEPOFFSETDEFINES_H

#include <QString>

namespace StepOffsetDefines {

/*!
 * \brief Перечень этапов Stage enum
 */
enum Stage
{
      stgWaiting = 0   //! Ожидание
    , stgCompensaton   //! Компенсация воздействия
    , stgReturn        //! Возврат в исходное состояние
};

///< Минимальное значение смещения при выборе диапазона отклонения
static const int MinValueOffset = 50;
///< Минимальная длительность переходного процесса в секундах
static const int MinLengthTransient = 3;
///< Время стартового участка в секундах для определения латентного периода
static const double StartFindTimeLatent = 0.2;
///< Допустимый разброс для латентного периода в мм +-
static const int DeltaLatenet = 5;
///< Допустимый разброс скорости
static const double Delta0Speed = 0.05;
///< Допустимый разброс статизма
static const double DeltaReady = 3.0;

///< Границы временных зон для анализа "коррекций"
static const double MotorCorrectTimeBoundLo = 0.1;
static const double MotorCorrectTimeBoundHi = 0.16;
static const double KognCorrectTimeBoundLo = 0.2;
static const double KognCorrectTimeBoundHi = 0.28;


static const QString S_KorrectKogn  = "Преобладание когнитивных коррекций";
static const QString S_KorrectMotor = "Преобладание моторных коррекций";
static const QString S_KorrectNo    = "Нет преобладания типа коррекции";

static const QString S_Insignificantly = "незначительно";
static const QString S_Moderate        = "умеренно";
static const QString S_Average         = "выражено";

}

#endif // STEPOFFSETDEFINES_H
