#ifndef SIGNALSDEFINES_H
#define SIGNALSDEFINES_H

#include <QObject>

namespace SignalsDefines
{

/*!
 * \brief Запись в канале стабилограммы StabRec struct
 */
struct StabRec
{
    double x;
    double y;
    StabRec() {}
    StabRec(std::tuple<double, double> params)
        : x(std::get<0>(params))
        , y(std::get<1>(params))
    {}
};

/*!
 * \brief Данные о прыжке JumpRec struct
 */
struct JumpRec
{
    double height;       ///< Высота
    double timeJump;     ///< Время прыжка
    JumpRec() {}
};

/*!
 * \brief Данные о шаге теппинг теста TeppingStepRec struct
 */
struct TeppingStepRec
{
    double timeContact;     ///< Время контакта
    TeppingStepRec() {}
    TeppingStepRec(double tc) : timeContact(tc) {}
};

/*!
 * \brief Данные о прыжке дроп теста DropJumpRec struct
 */
struct DropJumpRec
{
    double fallHeight;      ///< Высота спрыгивания
    double height;          ///< Высота прыжка
    double timeNoContact;   ///< Время безконтактной фазы
    double timeContact;     ///< Время контакта
    DropJumpRec() {}
};

/*!
 * \brief Данные о прыжке дроп теста с рассчитанными показателями DropJumpFactors struct
 */
struct DropJumpFactors : public DropJumpRec
{
    double power;
    double stiffness;
    double initialSpeed;
    double rsi;
    DropJumpFactors() : DropJumpRec() {}
};

}

#endif // SIGNALSDEFINES_H
