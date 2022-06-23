#ifndef SUMMARYDEFINES_H
#define SUMMARYDEFINES_H

#include <QCoreApplication>
#include <QString>
#include <QMap>

namespace SummaryDefines
{

/*!
 * \brief Режимы добавления теста в сводку The AddTestMode enum
 */
enum AddTestMode
{
      atmNone = -1
    , atmActive       ///< В активную
    , atmNew          ///< В новую
};

/*!
 * \brief Тип сводки The Kind enum
 */
enum Kind
{
      skNone = 0
    , skPrimary       ///< Первичные показатели
    , skAll           ///< Все показатели
};

/*!
 * \brief Структура данных об активной сводке The ActiveSummaryInfo struct
 */
struct ActiveSummaryInfo
{
    QString uidMethodic;
    Kind kind;

    ActiveSummaryInfo(const QString um, const Kind k)
        : uidMethodic(um), kind(k) {}
    ActiveSummaryInfo() {}
};

/*!
 * \brief Типы фильтров экспорта сводок The ExportModeFilters enum
 */
enum ExportModeFilters
{
      emfSummary = 0
    , emfText
};

static const QString SummarySuffix = "asmry";
static const QString emfnSummary = QCoreApplication::tr("Файлы сводок") + " *." + SummarySuffix + " (*." + SummarySuffix + ")";
static const QString emfnText = QCoreApplication::tr("Текстовые файлы") + " *.txt (*.txt)";

static QMap<ExportModeFilters, QString> exportModeFiltersNames{
    std::pair<ExportModeFilters, QString> (emfSummary, emfnSummary)
  , std::pair<ExportModeFilters, QString> (emfText, emfnText)
};


}

#endif // SUMMARYDEFINES_H
