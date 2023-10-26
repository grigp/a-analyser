#ifndef ACTIVEPERSONALPROGRAMEDITOR_H
#define ACTIVEPERSONALPROGRAMEDITOR_H

#include <QDialog>
#include <QJsonObject>
#include <QStandardItemModel>

#include "personalprogramdefines.h"

namespace Ui {
class ActivePersonalProgramEditor;
}

class PersonalProgram;
class SelectDailyProgramDialog;
class SelectMethodicDialog;

class ActivePersonalProgramEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ActivePersonalProgramEditor(QWidget *parent = nullptr);
    ~ActivePersonalProgramEditor() override;

    /*!
     * \brief Отображает данные индивидуальной программы
     * \param objPPAll - индивидуальная программа
     */
    void setPersonalProgram(const QJsonObject& objPPAll);

    /*!
     * \brief Возвращает отредактированную персональную программу
     */
    QJsonObject personalProgram() const;

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_selectDP(QModelIndex index);
    void on_selectT(QModelIndex index);

    void on_dpAdd();
    void on_dpDel();
    void on_dpMoveUp();
    void on_dpMoveDown();

    void on_testAdd();
    void on_testEdit();
    void on_testDel();
    void on_testMoveUp();
    void on_testMoveDown();

    void on_minTimeChanged(int idx);
    void on_maxTimeChanged(int idx);

private:
    Ui::ActivePersonalProgramEditor *ui;

    /*!
     * \brief Загрузка списка дневных программ
     */
    void initListDP();
    /*!
     * \brief Отображает дневную программу в виджетах формы
     * \param numDP - номер DP
     */
    void viewDP(const int numDP, const PersonalProgramDefines::DPCompletedValue complNext);

    void prepareParams();

    QModelIndex selectedDPIndex();
    QModelIndex selectedTestIndex();

    PersonalProgram* m_mdlPP {nullptr};  ///< Модель, хранящая данные индивидуальной программмы
    QStandardItemModel m_mdlDP;          ///< Отдельная модель для отображения списка дневных программ
    QStandardItemModel m_mdlT;           ///< Отдельная модель для отображения списка тестов

    SelectDailyProgramDialog* m_dlgSelDP {nullptr};
    SelectMethodicDialog* m_dlgSelMethod {nullptr};
};

#endif // ACTIVEPERSONALPROGRAMEDITOR_H
