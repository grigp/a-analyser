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

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // PATIENTSPROXYMODEL_H
