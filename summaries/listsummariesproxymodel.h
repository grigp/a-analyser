#ifndef LISTSUMMARIESPROXYMODEL_H
#define LISTSUMMARIESPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QUuid>

#include "summarydefines.h"

/*!
 * \brief Класс прокси модели списка сводок The ListSummariesProxyModel class
 * Позволяет выбрать сводки по методике, а также заданного типа
 */
class ListSummariesProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ListSummariesProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    void selectMetodic(const QString &metUid);

    void selectKind(const SummaryDefines::Kind kind);

private:
    QString m_metodicUid {QUuid().toString()};
    SummaryDefines::Kind m_kind {SummaryDefines::skNone};
};

#endif // LISTSUMMARIESPROXYMODEL_H
