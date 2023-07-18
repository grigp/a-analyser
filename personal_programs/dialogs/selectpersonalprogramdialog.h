#ifndef SELECTPERSONALPROGRAMDIALOG_H
#define SELECTPERSONALPROGRAMDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QItemSelectionModel>

namespace Ui {
class SelectPersonalProgramDialog;
}

/*!
 * \brief Диалог выбора индивидуальной программы The SelectPersonalProgramDialog class
 */
class SelectPersonalProgramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPersonalProgramDialog(QWidget *parent = nullptr);
    ~SelectPersonalProgramDialog() override;

    /*!
     * \brief Возвращает uid выбранной индивидуальной программы
     */
    QString personalProgramUid() const;

    /*!
     * \brief Возвращает индивидуальную программу в виде json объекта
     */
    QJsonObject personalProgram() const;

//    /*!
//     * \brief Возвращает индивидуальную программу в виде json объекта
//     */
//    QJsonObject personalProgram() const;

public slots:
    int exec() override;

private slots:
    void on_doubleClicked(const QModelIndex &index);
    void on_selectMetodicChanged(const QItemSelection &selected, const QItemSelection &deselected);


private:
    Ui::SelectPersonalProgramDialog *ui;

    QStandardItemModel m_mdlPrograms;
};

#endif // SELECTPERSONALPROGRAMDIALOG_H
