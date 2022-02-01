/*!
  *****************************************************************************************

  Файл установочных констант для сборки программы
  Для каждой сборки имеется свой файл
  Сборка - набор методик для определенных целей, напрмер для допуск контроля или для тренинга
  Этот файл предназначен для сборки для программы "Стань чемпионом"

  *****************************************************************************************
*/
#ifndef AANALYSERDEFINES_H
#define AANALYSERDEFINES_H

#include <QString>

namespace AAnalyserDefines
{

//! uid сборки
static const QString BuildUid = "{2932CDB1-5DA2-4243-A37A-F21A2FC1091B}";

//! Главная иконка приложения
static const QString MainIcon = ":/images/MainIcon_sch.ico";

//! Текст главного окна приложения
static const QString MainWindowTitle = "Стань чемпионом - стабилометрия";


//! Имя файла предустановленных методик. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsMetodicsFileName + '.json' - русская версия
//! PresetsMetodicsFileName + '_en_US.json - английская версия
static const QString PresetsMetodicsFileName = "metodics_st_champ";

//! Будет ли проходить автоимпорт БД
static const bool isAutoImportDatabases = false;

//! Название основного файла цветового оформления
static const QString MainStyleSheetFile = ":/qss/main_green.qss";

}

#endif // AANALYSERDEFINES_H
