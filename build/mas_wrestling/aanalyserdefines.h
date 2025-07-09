/*!
  *****************************************************************************************

  Файл установочных констант для сборки программы
  Для каждой сборки имеется свой файл
  Сборка - набор методик для определенных целей, напрмер для допуск контроля или для тренинга
  Этот файл предназначен для сборки динамической стабилизации IDS

  *****************************************************************************************
*/
#ifndef AANALYSERDEFINES_H
#define AANALYSERDEFINES_H

#include <QString>

namespace AAnalyserDefines
{

//! uid сборки
static const QString BuildUid = "{E995ED19-9EF3-4EC8-8FE2-F4C7D9AE57FF}";

//! Главная иконка приложения
static const QString MainIcon = ":/images/MainIcon_mw.ico";

//! Имя файла предустановленных методик. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsMetodicsFileName + '.json' - русская версия
//! PresetsMetodicsFileName + '_en_US.json - английская версия
static const QString PresetsMetodicsFileName = "metodics_mw";

//! Имя файла предустановленных подключений. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsConnectionFileName + '.json' - русская версия
//! PresetsConnectionFileName + '_en_US.json - английская версия
static const QString PresetsConnectionFileName = "connections_mw";

//! Будет ли проходить автоимпорт БД
static const bool isAutoImportDatabases = false;

//! Название основного файла цветового оформления
static const QString MainStyleSheetFile = ":/qss/main.qss";

}

#endif // AANALYSERDEFINES_H
