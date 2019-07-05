#include "patientsproxymodel.h"

#include "patientsmodel.h"

#include <QDebug>

PatientsProxyModel::PatientsProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

bool PatientsProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if (source_left.column() == PatientsModel::ColBorn && source_right.column() == PatientsModel::ColBorn)
    {
        auto sLeft = sourceModel()->data(source_left).toString();
        auto sRight = sourceModel()->data(source_right).toString();
        auto dtLeft = QDateTime::fromString(sLeft, "dd.MM.yyyy");
        auto dtRight = QDateTime::fromString(sRight, "dd.MM.yyyy");
        qDebug() << sLeft << sRight << "    " << dtLeft << dtRight;
        return dtLeft < dtRight;
    }
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
