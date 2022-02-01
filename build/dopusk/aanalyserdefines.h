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
static const QString BuildUid = "{FF360170-015F-4825-9CD1-BCDC5A16B9C5}";

//! Главная иконка приложения
static const QString MainIcon = ":/images/MainIcon_Dopusk.ico";

//! Текст главного окна приложения
static const QString MainWindowTitle = "Допусковый контроль А-Мед";


//! Имя файла предустановленных методик. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsMetodicsFileName + '.json' - русская версия
//! PresetsMetodicsFileName + '_en_US.json - английская версия
static const QString PresetsMetodicsFileName = "metodics_dopusk";

//! Будет ли проходить автоимпорт БД
static const bool isAutoImportDatabases = false;

static const QString MainStyleSheetFile = ":/qss/main_green.qss";
//static const QString MainStyleSheetFile = ":/qss/main.qss";
}

#endif // AANALYSERDEFINES_H
