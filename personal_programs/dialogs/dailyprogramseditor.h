#ifndef DAILYPROGRAMSEDITOR_H
#define DAILYPROGRAMSEDITOR_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DailyProgramsEditor;
}

class DailyProgramsEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DailyProgramsEditor(QWidget *parent = nullptr);
    ~DailyProgramsEditor();

private slots:
    void on_addTest();
    void on_editTest();
    void on_delTest();
    void on_moveTestUp();
    void on_moveTestDown();

    void on_dpAdd();
    void on_dpEdit();
    void on_dpDel();

private:
    Ui::DailyProgramsEditor *ui;

    QStandardItemModel m_mdlPrograms;
    QStandardItemModel m_mdlTests;
};

#endif // DAILYPROGRAMSEDITOR_H
