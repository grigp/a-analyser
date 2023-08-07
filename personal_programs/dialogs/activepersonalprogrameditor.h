#ifndef ACTIVEPERSONALPROGRAMEDITOR_H
#define ACTIVEPERSONALPROGRAMEDITOR_H

#include <QDialog>
#include <QJsonObject>
#include <QStandardItemModel>

namespace Ui {
class ActivePersonalProgramEditor;
}

class PersonalProgram;
class SelectDailyProgramDialog;

class ActivePersonalProgramEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ActivePersonalProgramEditor(QWidget *parent = nullptr);
    ~ActivePersonalProgramEditor();

    /*!
     * \brief Отображает данные индивидуальной программы
     * \param objPPAll - индивидуальная программа
     */
    void setPersonalProgram(const QJsonObject& objPPAll);

    /*!
     * \brief Возвращает отредактированную персональную программу
     */
    QJsonObject personalProgram() const;

private slots:
    void on_selectDP(QModelIndex index);
    void on_selectT(QModelIndex index);

    void on_dpAdd();
    void on_dpDel();
    void on_dpMoveUp();
    void on_dpMoveDown();

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
    void viewDP(const int numDP);

    void prepareParams();

    QModelIndex selectedDPIndex();

    PersonalProgram* m_mdlPP {nullptr};  ///< Модель, хранящая данные индивидуальной программмы
    QStandardItemModel m_mdlDP;          ///< Отдельная модель для отображения списка дневных программ
    QStandardItemModel m_mdlT;           ///< Отдельная модель для отображения списка тестов

    SelectDailyProgramDialog* m_dlgSelDP {nullptr};
};

#endif // ACTIVEPERSONALPROGRAMEDITOR_H
