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
static const QString BuildUid = "{52F2DAF9-307E-442D-A345-6A190D2CC08B}";

//! Главная иконка приложения
static const QString MainIcon = ":/images/MainIcon_DCI.ico";

//! Текст главного окна приложения
static const QString MainWindowTitle = "Анализ здровья по ИДС А-МЕД";


//! Имя файла предустановленных методик. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsMetodicsFileName + '.json' - русская версия
//! PresetsMetodicsFileName + '_en_US.json - английская версия
static const QString PresetsMetodicsFileName = "metodics_ids";

//! Имя файла предустановленных подключений. Это заголовок имени.
//! Полное имя формируется так:
//! PresetsConnectionFileName + '.json' - русская версия
//! PresetsConnectionFileName + '_en_US.json - английская версия
static const QString PresetsConnectionFileName = "connections_stab";

//! Будет ли проходить автоимпорт БД
static const bool isAutoImportDatabases = false;

//! Название основного файла цветового оформления
static const QString MainStyleSheetFile = ":/qss/main_red.qss";
}

#endif // AANALYSERDEFINES_H
