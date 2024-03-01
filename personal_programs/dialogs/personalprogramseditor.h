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
    ~PersonalProgramsEditor() override;

public slots:
    int exec() override;

protected:
    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_addDP();
    void on_doubleDP();
    void on_delDP();
    void on_moveDPUp();
    void on_moveDPDown();

    void on_ppAdd();
    void on_ppEdit();
    void on_ppDel();

    void on_selectPP(QModelIndex index);
    void on_changeMinTime(int idx);
    void on_changeMaxTime(int idx);

    void on_selectLogo();

    void on_splitterMoved(int,int);

private:
    Ui::PersonalProgramsEditor *ui;

    void prepareParams();
    void saveGeometry();

    SelectDailyProgramDialog* m_dlgSelDP {nullptr};
    QStandardItemModel m_mdlDP;
    QStandardItemModel m_mdlPP;
    QString m_logoFileName {""};
};

#endif // PERSONALPROGRAMSEDITOR_H
