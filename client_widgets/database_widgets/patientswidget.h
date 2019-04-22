#ifndef PATIENTSWIDGET_H
#define PATIENTSWIDGET_H

#include <QWidget>

class PatientsModel;

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

private:
    Ui::PatientsWidget *ui;

    PatientsModel *m_mdlPatients;
};

#endif // PATIENTSWIDGET_H
