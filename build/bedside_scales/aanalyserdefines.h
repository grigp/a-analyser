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
static const QString BuildUid = "{45F28995-3F75-461F-83A5-7D36EF6D97C5}";

//! Главная иконка приложения
static const QString MainIcon = ":/images/MainIcon_bss.ico";

//! Текст главного окна приложения
static const QString MainWindowTitle = "А-Мед. Прикроватные весы";

//! Имя файла предустановленных методик. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsMetodicsFileName + '.json' - русская версия
//! PresetsMetodicsFileName + '_en_US.json - английская версия
static const QString PresetsMetodicsFileName = "metodics_bss";

//! Имя файла предустановленных подключений. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsConnectionFileName + '.json' - русская версия
//! PresetsConnectionFileName + '_en_US.json - английская версия
static const QString PresetsConnectionFileName = "connections_bss";

//! Будет ли проходить автоимпорт БД
static const bool isAutoImportDatabases = false;

//! Название основного файла цветового оформления
static const QString MainStyleSheetFile = ":/qss/main.qss";

}

#endif // AANALYSERDEFINES_H
