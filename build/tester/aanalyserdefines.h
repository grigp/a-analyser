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

//! Имя файла предустановленных методик. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsMetodicsFileName + '.json' - русская версия
//! PresetsMetodicsFileName + '_en_US.json - английская версия
static const QString PresetsMetodicsFileName = "metodics_tester";

//! Будет ли проходить автоимпорт БД
static const bool isAutoImportDatabases = false;

}

#endif // AANALYSERDEFINES_H
