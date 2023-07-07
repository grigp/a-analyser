#ifndef DAILYPROGRAMSEDITOR_H
#define DAILYPROGRAMSEDITOR_H

#include <QDialog>
#include <QUuid>
#include <QStandardItemModel>
#include <QJsonObject>

namespace Ui {
class DailyProgramsEditor;
}

class SelectMethodicDialog;

/*!
 * \brief Класс диалога редактирования дневных программ The DailyProgramsEditor class
 */
class DailyProgramsEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DailyProgramsEditor(QWidget *parent = nullptr);
    ~DailyProgramsEditor() override;

public slots:
    int exec() override;

private slots:
    void on_addTest();
    void on_editTest();
    void on_delTest();
    void on_moveTestUp();
    void on_moveTestDown();

    void on_dpAdd();
    void on_dpEdit();
    void on_dpDel();

    void on_selectDP(QModelIndex index);

private:
    Ui::DailyProgramsEditor *ui;

    /*!
     * \brief Собирает дневную программу из виджетов формы в JsonObject. DP хранятся в JsonObject-ах
     * \return собранную DP
     */
    QJsonObject compileDP();

    /*!
     * \brief Отображает дневную программу в виджетах формы
     * \param objDP - хранимая DP
     */
    void viewDP(const QJsonObject& objDP);

    SelectMethodicDialog* m_dlgSelMethod {nullptr};

    QStandardItemModel m_mdlPrograms;
    QStandardItemModel m_mdlTests;
};

#endif // DAILYPROGRAMSEDITOR_H
