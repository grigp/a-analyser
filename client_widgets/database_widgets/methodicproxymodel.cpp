#include "methodicproxymodel.h"

#include "metodicsmodel.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "databasewigetdefines.h"

#include <QDebug>

MethodicProxyModel::MethodicProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

bool MethodicProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex idxPat = sourceModel()->index(source_row,
                                              DatabaseWidgetDefines::MetodicsModel::ColName,
                                              source_parent);
    auto metUid = idxPat.data(DatabaseWidgetDefines::MetodicsModel::MetodicUidRole).toString();
    auto met = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(metUid);

    return ((m_selectMetodicKindUid != QUuid().toString()) && (m_selectMetodicKindUid == met.kindUid)) ||
            (m_selectMetodicKindUid == QUuid().toString());
}

void MethodicProxyModel::selectMetodicKind(const QString &kindUid)
{
    m_selectMetodicKindUid = kindUid;
    invalidate();
}
