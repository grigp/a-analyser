#include "listsummariesmodel.h"

ListSummariesModel::ListSummariesModel(QObject *parent)
    : QStandardItemModel (parent)
{
}

QString ListSummariesModel::columnTitle(const ListSummariesModel::Columns column)
{
    QMap<Columns, QString> titles {
        std::pair<Columns, QString> (colName, tr("Сводка"))
      , std::pair<Columns, QString> (colMethodic, tr("Методика"))
      , std::pair<Columns, QString> (colKind, tr("Показатели"))
    };
    return titles.value(column);
}

