#ifndef SUMMARYDEFINES_H
#define SUMMARYDEFINES_H

namespace SummaryDefines
{

/*!
 * \brief Режимы добавления теста в сводку The AddTestMode enum
 */
enum AddTestMode
{
      atmNone = -1
    , atmExists       ///< В существующую
    , atmNew          ///< В новую
    , atmActive       ///< В активную
};

/*!
 * \brief Тип сводки The Kind enum
 */
enum Kind
{
      skNone = 0
    , skPrimary       ///< Первичные показатели
    , skAll           ///< Все показатели
};

}

#endif // SUMMARYDEFINES_H
