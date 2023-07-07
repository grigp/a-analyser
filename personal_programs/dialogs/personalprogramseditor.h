#ifndef PERSONALPROGRAMSEDITOR_H
#define PERSONALPROGRAMSEDITOR_H

#include <QDialog>

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

private slots:
    void on_addDP();

private:
    Ui::PersonalProgramsEditor *ui;

    SelectDailyProgramDialog* m_dlgSelDP {nullptr};
};

#endif // PERSONALPROGRAMSEDITOR_H
