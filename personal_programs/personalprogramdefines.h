#ifndef PERSONALPROGRAMDEFINES_H
#define PERSONALPROGRAMDEFINES_H

#include <QObject>
#include <QColor>
#include <QApplication>

#include "databasewigetdefines.h"


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

/*!
 * \brief Роли таблицы индивидуальных программ The TablePPRoles enum
 */
enum TablePPRoles
{
    PPRole = Qt::UserRole + 1   ///< Объект json индивидуальной программы
};



///< ---------------------------- Параметры индивидуальной программы -------------------------------------------
/*!
 * \brief Параметр индивидуальной программы: Минимальное время между дневными программами The MinTimeBetweenDP enum
 */
enum MinTimeBetweenDP
{
      minOtherDay = -1
    , minUndefined = 0
    , minOneHour = 1
    , minTwoHours = 2
    , minFiveHours = 5
    , minTwelveHours = 12
    , minDay = 24
    , minTwoDays = 48
    , minFiveDays = 120
};

/*!
 * \brief Параметр индивидуальной программы: Максимальное время между дневными программами The MaxTimeBetweenDP enum
 */
enum MaxTimeBetweenDP
{
      maxUndefined = 0
    , maxOneHour = 1
    , maxTwoHours = 2
    , maxFiveHours = 5
    , maxTwelveHours = 12
    , maxOneDay = 24
    , maxTwoDays = 48
    , maxThreeDays = 72
    , maxWeek = 168
    , maxMonth = 720
};


/*!
 * \brief Название значений параметра индивидуальной программы: Минимальное время между дневными программами The MinTimeBetweenDPNames enum
 */
static QMap<MinTimeBetweenDP, QString> MinTimeBetweenDPNames
{
      std::pair<MinTimeBetweenDP, QString> (minOtherDay, QApplication::tr("На другой день"))
    , std::pair<MinTimeBetweenDP, QString> (minUndefined, QApplication::tr("Нет"))
    , std::pair<MinTimeBetweenDP, QString> (minOneHour, QApplication::tr("Один час"))
    , std::pair<MinTimeBetweenDP, QString> (minTwoHours, QApplication::tr("Два часа"))
    , std::pair<MinTimeBetweenDP, QString> (minFiveHours, QApplication::tr("Пять часов"))
    , std::pair<MinTimeBetweenDP, QString> (minTwelveHours, QApplication::tr("Двенадцать часов"))
    , std::pair<MinTimeBetweenDP, QString> (minDay, QApplication::tr("Одни сутки"))
    , std::pair<MinTimeBetweenDP, QString> (minTwoDays, QApplication::tr("Двое суток"))
    , std::pair<MinTimeBetweenDP, QString> (minFiveDays, QApplication::tr("Пять суток"))
};

/*!
 * \brief Название значений параметра индивидуальной программы: Максимальное время между дневными программами The MaxTimeBetweenDPNames enum
 */
static QMap<MaxTimeBetweenDP, QString> MaxTimeBetweenDPNames
{
      std::pair<MaxTimeBetweenDP, QString> (maxUndefined, QApplication::tr("Нет"))
    , std::pair<MaxTimeBetweenDP, QString> (maxOneHour, QApplication::tr("Один час"))
    , std::pair<MaxTimeBetweenDP, QString> (maxTwoHours, QApplication::tr("Два часа"))
    , std::pair<MaxTimeBetweenDP, QString> (maxFiveHours, QApplication::tr("Пять часов"))
    , std::pair<MaxTimeBetweenDP, QString> (maxTwelveHours, QApplication::tr("Двенадцать часов"))
    , std::pair<MaxTimeBetweenDP, QString> (maxOneDay, QApplication::tr("Одни сутки"))
    , std::pair<MaxTimeBetweenDP, QString> (maxTwoDays, QApplication::tr("Двое суток"))
    , std::pair<MaxTimeBetweenDP, QString> (maxThreeDays, QApplication::tr("Трое суток"))
    , std::pair<MaxTimeBetweenDP, QString> (maxWeek, QApplication::tr("Неделя"))
    , std::pair<MaxTimeBetweenDP, QString> (maxMonth, QApplication::tr("Месяц"))
};

/*!
 * \brief Перечень значений параметра индивидуальной программы: Минимальное время между дневными программами The MinTimeBetweenDPList enum
 */
static QList<MinTimeBetweenDP> MinTimeBetweenDPList
{
    minOtherDay, minUndefined, minOneHour, minTwoHours, minFiveHours, minTwelveHours, minDay, minTwoDays, minFiveDays
};

/*!
 * \brief Перечень значений параметра индивидуальной программы: Минимальное время между дневными программами The MaxTimeBetweenDPList enum
 */
static QList<MaxTimeBetweenDP> MaxTimeBetweenDPList
{
    maxUndefined, maxOneHour, maxTwoHours, maxFiveHours, maxTwelveHours, maxOneDay, maxTwoDays, maxThreeDays, maxWeek, maxMonth
};

namespace PersonalProgram
{

/*!
 * \brief Колонки модели The Columns enum
 */
enum Columns
{
      ColDailyProgram = 0  ///< Колонка заголовка дневной программы Показывает дату и время проведения
    , ColTest0             ///< Колонка первого теста
};

/*!
 * \brief Роли для доступа к данным модели The Roles enum
 */
enum Roles
{
      DateTimeRole = Qt::UserRole + 1  ///< Дата и время начала проведения дневной программы. Колонка ColDailyProgram. Тип QString
    , DPUidRole                        ///< uid дневной программы. Колонка ColDailyProgram. Тип QString
    , DPNameRole                       ///< Название дневной программы. Колонка ColDailyProgram. Тип QString
    , DPDateTimeRole                   ///< Дата и время начала дневной программы. Колонка ColDailyProgram. Тип QString. Формат dd.MM.yyyy hh:mm
    , MethodUidRole                    ///< uid методики. Колонки ColTest0, ... Тип QString
    , TestUidRole                      ///< uid проведенного теста. Колонки ColTest0, ... Тип QString.
                                                                  ///< По наличию значения можно определить, проведен ли тест
    , ParamsRole                       ///< Параметры методики для проведения теста. Колонки ColTest0, ... Тип QJsonObject

    , PersonalProgramUserRole          ///< Начиная с этой роли можно дополнительно свои назначать
};

}



//! Дополнительные роли
enum PatientTableAdvancedRoles
{
    //! Роль для доступа к виджету индивидуальной программы из виджета индивидуальных программ
    //! Хранится указатель на виджет
    //! Применимо к колонке DatabaseWidgetDefines::PatientsModel::ColFio
    PersonalProgramWidgetRole = DatabaseWidgetDefines::PatientsModel::PatientTableUserRole + 1
};

/*!
 * \brief Значение параметра выполненности ДП
 */
enum DPCompletedValue
{
      dpcvNotStarted = 0  ///< Не начата
    , dpcvInterrupted     ///< Прервана
    , dpcvCompleted       ///< Завершена
};

/*!
 * \brief Роли для доступа к данным в диалоге редактора назначенных программ
 */
enum DPAdvancedRoles
{
    DPCompletedRole = PersonalProgramDefines::PersonalProgram::PersonalProgramUserRole + 1  ///< Значение типа DPCompletedValue
};


}


#endif // PERSONALPROGRAMDEFINES_H
