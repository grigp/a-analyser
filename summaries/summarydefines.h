#ifndef SUMMARYDEFINES_H
#define SUMMARYDEFINES_H

#include <QString>

namespace SummaryDefines
{

/*!
 * \brief Режимы добавления теста в сводку The AddTestMode enum
 */
enum AddTestMode
{
      atmNone = -1
    , atmActive       ///< В активную
    , atmNew          ///< В новую
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

/*!
 * \brief Структура данных об активной сводке The ActiveSummaryInfo struct
 */
struct ActiveSummaryInfo
{
    QString uidMethodic;
    Kind kind;

    ActiveSummaryInfo(const QString um, const Kind k)
        : uidMethodic(um), kind(k) {}
    ActiveSummaryInfo() {}
};

}

#endif // SUMMARYDEFINES_H
