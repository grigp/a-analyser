#ifndef STEPOFFSETDEFINES_H
#define STEPOFFSETDEFINES_H

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

static const int MinValueOffset = 50;

}

#endif // STEPOFFSETDEFINES_H
