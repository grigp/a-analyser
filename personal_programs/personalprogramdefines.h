#ifndef PERSONALPROGRAMDEFINES_H
#define PERSONALPROGRAMDEFINES_H

#include <QObject>
#include <QApplication>

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
    , std::pair<MinTimeBetweenDP, QString> (minFiveHours, QApplication::tr("Три часа"))
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

}


#endif // PERSONALPROGRAMDEFINES_H