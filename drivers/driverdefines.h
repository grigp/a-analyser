#ifndef DRIVERDEFINES_H
#define DRIVERDEFINES_H

#include <QCoreApplication>
#include <QMap>

namespace DriverDefines {

/*!
 * \brief Коды внутренних ошибок в драйверах The ErrorCodes enum
 */
enum ErrorCodes
{
      EC_NoError = 0                   ///< Нет ошибки
    , EC_NoData = 1                    ///< Нет данных
    , EC_LossData = 2                  ///< Потеря данных (пропуск пакета)
    , EC_MarkerIinsidePackage = 101    ///< Маркер внутри пакета
    , EC_User = 100                    ///< Ошибки конкретных драйверов
};


///! Возвращает текст ошибки по коду
static QMap<ErrorCodes, QString> errorName {
    std::pair<ErrorCodes, QString> (EC_NoError, QCoreApplication::tr("Нет ошибки"))
  , std::pair<ErrorCodes, QString> (EC_NoData, QCoreApplication::tr("Нет данных"))
  , std::pair<ErrorCodes, QString> (EC_LossData, QCoreApplication::tr("Потеря данных (пропуск пакета)"))
  , std::pair<ErrorCodes, QString> (EC_MarkerIinsidePackage, QCoreApplication::tr("Маркер внутри пакета"))
};


}

#endif // DRIVERDEFINES_H
