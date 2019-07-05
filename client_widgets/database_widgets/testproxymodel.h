#ifndef TESTPROXYMODEL_H
#define TESTPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class TestProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TestProxyModel(QObject *parent = nullptr);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private slots:
    void onSelectPatient(const QString &patientUid);
    void onSelectMetodic(const QString &metodicUid);

private:
    QString m_selectPatientUid = "";
    QString m_selectMetodicUid = "";

};

#endif // TESTPROXYMODEL_H
