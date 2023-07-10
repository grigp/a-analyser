#ifndef SELECTDAILYPROGRAMDIALOG_H
#define SELECTDAILYPROGRAMDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QItemSelectionModel>


namespace Ui {
class SelectDailyProgramDialog;
}

/*!
 * \brief Класс диалога выбора дневной программы The SelectDailyProgramDialog class
 */
class SelectDailyProgramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDailyProgramDialog(QWidget *parent = nullptr);
    ~SelectDailyProgramDialog() override;

    /*!
     * \brief Возвращает uid выбранной дневной программы
     */
    QString dailyProgramUid() const;

    /*!
     * \brief Возвращает дневную программу в виде json объекта
     */
    QJsonObject dailyProgram() const;

public slots:
    int exec() override;

private slots:
    void on_doubleClicked(const QModelIndex &index);
    void on_selectMetodicChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    Ui::SelectDailyProgramDialog *ui;

    QStandardItemModel m_mdlPrograms;
};

#endif // SELECTDAILYPROGRAMDIALOG_H
