#ifndef PATIENTPROGRAMWIDGET_H
#define PATIENTPROGRAMWIDGET_H

#include <QWidget>
#include <QModelIndex>

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

    /*!
     * \brief Возвращает индекс выбранного итема
     */
    QModelIndex selectedItem() const {return m_selectedItem;}

private slots:
    void on_selectItem(QModelIndex idx);

private:
    Ui::PatientProgramWidget *ui;

    PersonalProgram* m_pp {nullptr};
    QModelIndex m_selectedItem {QModelIndex()};
};

#endif // PATIENTPROGRAMWIDGET_H
