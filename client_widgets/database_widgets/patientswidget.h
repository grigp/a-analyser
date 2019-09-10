#ifndef PATIENTSWIDGET_H
#define PATIENTSWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QItemSelectionModel>

class PatientsModel;
class PatientsProxyModel;

namespace Ui {
class PatientsWidget;
}

class PatientsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientsWidget(QWidget *parent = 0);
    ~PatientsWidget();

    void onDbConnect();

private slots:
    void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void selectPatient(const QModelIndex index);

    /*!
     * \brief Добавление пациента
     */
    void addPatient();

    /*!
     * \brief Редактирование записи о пациенте
     */
    void editPatient();

    /*!
     * \brief Удаление записи о пациенте
     */
    void removePatient();

    void unselect();

private:
    Ui::PatientsWidget *ui;

    PatientsModel *m_mdlPatients;
    PatientsProxyModel *m_pmdlPatients;
};

#endif // PATIENTSWIDGET_H
