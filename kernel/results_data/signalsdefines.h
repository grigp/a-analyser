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
    double height;          ///< Высота
    double timeContact;     ///< Время контакта
    JumpRec() {}
};


}

#endif // SIGNALSDEFINES_H
