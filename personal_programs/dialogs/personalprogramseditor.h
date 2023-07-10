#ifndef PERSONALPROGRAMSEDITOR_H
#define PERSONALPROGRAMSEDITOR_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class PersonalProgramsEditor;
}

class SelectDailyProgramDialog;

/*!
 * \brief Класс диалога редактора индивидуальных норм The PersonalProgramsEditor class
 */
class PersonalProgramsEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PersonalProgramsEditor(QWidget *parent = nullptr);
    ~PersonalProgramsEditor();

public slots:
    int exec() override;

private slots:
    void on_addDP();
    void on_delDP();
    void on_moveDPUp();
    void on_moveDPDown();

    void on_ppAdd();
    void on_ppEdit();
    void on_ppDel();

    void on_selectPP(QModelIndex index);

private:
    Ui::PersonalProgramsEditor *ui;

    void prepareParams();

    SelectDailyProgramDialog* m_dlgSelDP {nullptr};
    QStandardItemModel m_mdlDP;
    QStandardItemModel m_mdlPP;
};

#endif // PERSONALPROGRAMSEDITOR_H
