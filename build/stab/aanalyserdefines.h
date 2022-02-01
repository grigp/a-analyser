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
static const QString BuildUid = "{76E5FB17-34FA-4415-9F9E-6F791D905CB6}";

//! Главная иконка приложения
static const QString MainIcon = ":/images/MainIcon_stab.ico";

//! Текст главного окна приложения
static const QString MainWindowTitle = "Стабилометрия А-МЕД";


//! Имя файла предустановленных методик. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsMetodicsFileName + '.json' - русская версия
//! PresetsMetodicsFileName + '_en_US.json - английская версия
static const QString PresetsMetodicsFileName = "metodics_stab";

//! Будет ли проходить автоимпорт БД
static const bool isAutoImportDatabases = true;

//! Название основного файла цветового оформления
static const QString MainStyleSheetFile = ":/qss/main.qss";
//static const QString MainStyleSheetFile = ":/qss/main_yellow.qss";
//static const QString MainStyleSheetFile = ":/qss/main_red.qss";
//static const QString MainStyleSheetFile = ":/qss/main_green.qss";

}

#endif // AANALYSERDEFINES_H
