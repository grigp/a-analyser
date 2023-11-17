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
     * \brief Загружает индивидуальную программу по ее uid
     * \param patientUid - uid индивидуальной программы
     */
    void assignPersonalProgram(const QString& uidPP);

    /*!
     * \brief Возвращает индекс выбранного итема
     */
    QModelIndex selectedIndex() const {return m_selectedIndex;}

    /*!
     * \brief Возвращает название индивидуальной программы
     */
    QString namePP() const;

signals:
    void selectItem(QModelIndex idx);

private slots:
    void on_selectItem(QModelIndex idx);

private:
    Ui::PatientProgramWidget *ui;

    PersonalProgram* m_pp {nullptr};
    QModelIndex m_selectedIndex {QModelIndex()};
};

#endif // PATIENTPROGRAMWIDGET_H
