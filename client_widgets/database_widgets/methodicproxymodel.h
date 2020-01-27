#ifndef METHODICPROXYMODEL_H
#define METHODICPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QUuid>


class MethodicProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MethodicProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    void selectMetodicKind(const QString &kindUid);

private:
    QString m_selectMetodicKindUid = QUuid().toString();
};

#endif // METHODICPROXYMODEL_H
