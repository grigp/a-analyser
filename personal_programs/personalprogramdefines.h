#ifndef PERSONALPROGRAMDEFINES_H
#define PERSONALPROGRAMDEFINES_H

#include <QObject>

namespace PersonalProgramDefines
{

/*!
 * \brief Роли таблицы тестов The TableTestsRoles enum
 */
enum TableTestsRoles
{
      MethodUidRole = Qt::UserRole + 1  ///< uid методики
    , MethodParamsRole                  ///< параметры методики
};

/*!
 * \brief Роли таблицы дневных программ The TableDPRoles enum
 */
enum TableDPRoles
{
    DPRole = Qt::UserRole + 1   ///< Объект json дневной программы
};

}

#endif // PERSONALPROGRAMDEFINES_H
