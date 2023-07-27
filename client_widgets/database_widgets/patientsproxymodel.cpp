#include "patientsproxymodel.h"

#include "patientsmodel.h"

#include <QDebug>

PatientsProxyModel::PatientsProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

bool PatientsProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex idxFio = sourceModel()->index(source_row,
                                              PatientsModel::ColFio,
                                              source_parent);
    auto fio = idxFio.data().toString();
    auto ppUid = idxFio.data(PatientsModel::PatientPPUidRole).toString();

    bool isSPPMode = true;
    if (m_sppMode == sppFree)
        isSPPMode = (ppUid == "");
    else
    if (m_sppMode == sppActive)
        isSPPMode = (ppUid != "");

    return ((m_filerValue == "") || fio.contains(m_filerValue, Qt::CaseInsensitive)) &&
            isSPPMode;
}

bool PatientsProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if (source_left.column() == PatientsModel::ColBorn && source_right.column() == PatientsModel::ColBorn)
    {
        auto sLeft = sourceModel()->data(source_left).toString();
        auto sRight = sourceModel()->data(source_right).toString();
        auto dtLeft = QDateTime::fromString(sLeft, "dd.MM.yyyy");
        auto dtRight = QDateTime::fromString(sRight, "dd.MM.yyyy");
        return dtLeft < dtRight;
    }
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

void PatientsProxyModel::setFilterValue(const QString &value)
{
    m_filerValue = value;
    invalidate();
}

void PatientsProxyModel::setShowPPMode(PatientsProxyModel::ShowByPersonalProgram sppMode)
{
    m_sppMode = sppMode;
    invalidate();
}
