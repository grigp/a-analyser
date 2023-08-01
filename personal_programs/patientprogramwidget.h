#ifndef PATIENTPROGRAMWIDGET_H
#define PATIENTPROGRAMWIDGET_H

#include <QWidget>

namespace Ui {
class PatientProgramWidget;
}

class PersonalProgram;

/*!
 * \brief Класс виджета для отображения индивидуальной программы для пациента The PatientProgramWidget class
 */
class PatientProgramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientProgramWidget(QWidget *parent = nullptr);
    ~PatientProgramWidget();

    /*!
     * \brief Загружает индивидуальную программу для пациента
     * \param patientUid - uid пациента
     */
    void assignPersonalProgram(const QString& patientUid);

private:
    Ui::PatientProgramWidget *ui;

    PersonalProgram* m_pp {nullptr};
};

#endif // PATIENTPROGRAMWIDGET_H
