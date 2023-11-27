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
    explicit PatientsWidget(QWidget *parent = nullptr);
    ~PatientsWidget();

    void onDbConnect();

    void onShow();
    void onHide();

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

    void on_editSearchString(const QString &value);

    void on_applicationParamChanged(const QString &group, const QString &param, const QVariant &value);

    /*!
     * \brief Обработка сигналов о назначении индивидуальной программы для пациента
     * \param patientUid - uid пациента
     * \param ppUid - uid индивидуальной программы
     */
    void on_assignPPForPatient(const QString& patientUid, const QString& ppUid);

    /*!
     * \brief Обработка сигналов о снятии индивидуальной программы для пациента
     * \param patientUid - uid пациента
     */
    void on_cancelPPForPatient(const QString& patientUid);

    void on_viewModePP();

//protected:
//    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::PatientsWidget *ui;

    void onePatientHandle();

    PatientsModel* patientsModel() const;
    PatientsProxyModel* patientsProxyModel() const;

    bool m_isOnePatientMode {false};
    QString m_onePatientFIO {""};

//    PatientsModel *m_mdlPatients;
//    PatientsProxyModel *m_pmdlPatients;
};

#endif // PATIENTSWIDGET_H
