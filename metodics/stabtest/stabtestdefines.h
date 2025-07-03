#ifndef STABTESTDEFINES_H
#define STABTESTDEFINES_H

#include <QCoreApplication>
#include <QString>
#include <QMap>

namespace StabTestDefines
{
/*!
 * \brief Перечисление окон пациента The PatientWindows enum
 */
enum PatientWindows
{
      pwNone = 0
    , pwColors
    , pwSound
    , pwTarget
    , pwZones
    , pwLines
};

/*!
 * \brief Роли для модели таблицы нормативов для теста Ромберга
 */
enum RombergNormsTableRoles
{
      NormRole = Qt::UserRole + 1   ///< Норма. int. В колонках значений
    , ValueRole                     ///< Значение показателя. double. В колонках значений
    , FormatRole                    ///< Формат значения, кол-во знаков после запятой. int. В колонках значений
};


}

#endif // STABTESTDEFINES_H
