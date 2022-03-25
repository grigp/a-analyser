#ifndef PATIENTSPROXYMODEL_H
#define PATIENTSPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

/*!
 * \brief Класс прокси модели для списка пациентов PatientsProxyModel class
 */
class PatientsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PatientsProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

    void setFilterValue(const QString &value);

private:
    QString m_filerValue {""};
};

#endif // PATIENTSPROXYMODEL_H
