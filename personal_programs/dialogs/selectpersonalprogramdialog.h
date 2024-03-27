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
    /*!
     * \brief Режим выбора ИП The SelectMode enum
     */
    enum SelectMode
    {
          smNone = -1     ///< Не задан
        , smDefault       ///< Из предустановленных
        , smFromFile      ///< Из файла
    };

    explicit SelectPersonalProgramDialog(QWidget *parent = nullptr);
    ~SelectPersonalProgramDialog() override;

    /*!
     * \brief Возвращает режим выбора ИП
     */
    SelectMode mode() const;

    /*!
     * \brief Возвращает uid выбранной индивидуальной программы из списка в режиме smDefault
     */
    QString personalProgramUid() const;

    /*!
     * \brief Возвращает имя файла сохраненной ИП в режиме smFromFile
     */
    QString fileName() const;

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

protected:
    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_doubleClicked(const QModelIndex &index);
    void on_selectMetodicChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void on_selectFileName();
    void on_clickedMode();
    void on_changedFileName(QString fn);

private:
    Ui::SelectPersonalProgramDialog *ui;

    void setEnabledOK();

    QStandardItemModel m_mdlPrograms;
};

#endif // SELECTPERSONALPROGRAMDIALOG_H
