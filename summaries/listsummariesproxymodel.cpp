#include "listsummariesproxymodel.h"

#include <QDebug>

#include "listsummariesmodel.h"

ListSummariesProxyModel::ListSummariesProxyModel(QObject *parent)
    : QSortFilterProxyModel (parent)
{

}

bool ListSummariesProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex idxMet = sourceModel()->index(source_row,
                                              ListSummariesModel::colMethodic,
                                              source_parent);
    auto metUid = idxMet.data(ListSummariesModel::MethodicRole).toString();

    QModelIndex idxKind = sourceModel()->index(source_row,
                                               ListSummariesModel::colKind,
                                               source_parent);
    auto kind = static_cast<SummaryDefines::Kind>(idxKind.data(ListSummariesModel::KindRole).toInt());

    bool isMethodic = ((m_metodicUid != QUuid().toString()) && (m_metodicUid == metUid)) ||
                       (m_metodicUid == QUuid().toString());
    bool isKind = ((m_kind != SummaryDefines::skNone) && (m_kind == kind)) ||
                   (m_kind == SummaryDefines::skNone);

    return isMethodic && isKind;
}

void ListSummariesProxyModel::selectMetodic(const QString &metUid)
{
    m_metodicUid = metUid;
    invalidate();
}

void ListSummariesProxyModel::selectKind(const SummaryDefines::Kind kind)
{
    m_kind = kind;
    invalidate();
}
