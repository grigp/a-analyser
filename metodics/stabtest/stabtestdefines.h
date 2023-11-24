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
 * \brief Названия окон пациента The PatientWindowsName
 */
static QMap<PatientWindows, QString> PatientWindowsName {
      std::pair<PatientWindows, QString> (pwNone, QCoreApplication::tr("нет"))
    , std::pair<PatientWindows, QString> (pwColors, QCoreApplication::tr("Цветные круги"))
    , std::pair<PatientWindows, QString> (pwSound, QCoreApplication::tr("Звуковые сигналы"))
    , std::pair<PatientWindows, QString> (pwTarget, QCoreApplication::tr("Мишень"))
    , std::pair<PatientWindows, QString> (pwZones, QCoreApplication::tr("С выделенной зоной"))
    , std::pair<PatientWindows, QString> (pwLines, QCoreApplication::tr("Движение полос"))
};

}

#endif // STABTESTDEFINES_H
