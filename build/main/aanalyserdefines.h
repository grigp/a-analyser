/*!
  *****************************************************************************************

  Файл установочных констант для сборки программы
  Для каждой сборки имеется свой файл
  Сборка - набор методик для определенных целей, напрмер для допуск контроля или для тренинга
  Этот файл предназначен для общей сборки

  *****************************************************************************************
*/
#ifndef AANALYSERDEFINES_H
#define AANALYSERDEFINES_H

#include <QString>

namespace AAnalyserDefines
{

//! uid сборки
static const QString BuildUid = "{CA26008C-79DE-4DD7-A1A9-4FACAF79A283}";

//! Имя файла предустановленных методик. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsMetodicsFileName + '.json' - русская версия
//! PresetsMetodicsFileName + '_en_US.json - английская версия
static const QString PresetsMetodicsFileName = "metodics";

//! Будет ли проходить автоимпорт БД
static const bool isAutoImportDatabases = true;

//! Название основного файла цветового оформления
static const QString MainStyleSheetFile = ":/qss/main.qss";
//static const QString MainStyleSheetFile = ":/qss/main_red.qss";

}

#endif // AANALYSERDEFINES_H
