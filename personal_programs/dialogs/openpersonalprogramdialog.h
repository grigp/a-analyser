#ifndef OPENPERSONALPROGRAMDIALOG_H
#define OPENPERSONALPROGRAMDIALOG_H

#include <QDialog>

namespace Ui {
class OpenPersonalProgramDialog;
}

/*!
 * \brief Диалог завершения назначения индивидуальной программы The OpenPersonalProgramDialog class
 * Предлагает открыть назначенную персональную программу
 * В случае необходимости открытия возвращает QDialog::Accepted, иначе QDialog::Rejected
 */
class OpenPersonalProgramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenPersonalProgramDialog(QWidget *parent = nullptr);
    ~OpenPersonalProgramDialog();

    /*!
     * \brief Показывает ФИО пациента
     */
    void setPatientFio(const QString& fio);

    /*!
     * \brief Показывает название индивидуальной программы и логотип
     */
    void setPersonalProgram(const QString& name, const QPixmap& pic);

private:
    Ui::OpenPersonalProgramDialog *ui;
};

#endif // OPENPERSONALPROGRAMDIALOG_H
