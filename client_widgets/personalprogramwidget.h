#ifndef PERSONALPROGRAMWIDGET_H
#define PERSONALPROGRAMWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QItemSelectionModel>

#include "clientwidget.h"
#include "metodicdefines.h"

namespace Ui {
class PersonalProgramWidget;
}

namespace ClientWidgets
{
    static const QString uidPersonalProgramWidgetUid = "CW_PersonalProgram";
}

class PatientsModel;
class PatientsProxyModel;
class PatientProgramWidget;

class PersonalProgramWidget : public ClientWidget
{
    Q_OBJECT

public:
    explicit PersonalProgramWidget(QWidget *parent = nullptr);
    ~PersonalProgramWidget() override;

    /*!
     * \brief Виртуальный метод, возвращающий уникальный идентификатор виджета
     */
    QString uid() override;

    /*!
     * \brief Виртуальный метод, возвращающий название виджета
     */
    QString name() override;

    void onDBConnect() override;

    /*!
     * \brief Виртуальный метод, возвращающий true, если допустимы внешние элементы управления (глобальное меню и т.д.)
     */
    bool isExternalControl() override {return true;}

    /*!
     * \brief Виртуальный метод, возвращающий true, если виджет разрешает закрыться приложению
     * Чтобы запретить закрытие, нужно вернуть false
     */
    bool isCloseProgramQuery() override {return true;}

    /*!
     * \brief Вызывается после показа виджета
     */
    void onShow() override;
    /*!
     * \brief Вызывается перед прятанием виджета
     */
    void onHide() override;

private slots:
    void on_splitterMoved(int,int);

    void on_run();
    void on_delete();
    void on_params();

    void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void selectPatient(const QModelIndex index);

    void on_selectPatient(const QString &patientUid);

private:
    Ui::PersonalProgramWidget *ui;

    QStandardItem* appendLine(const QString uidPat,
                              const QString uidPPAssigned,
                              const QJsonObject& objPP = QJsonObject(),
                              QStandardItem* root = nullptr);
    void load();

//    void showPersonalProgram(const QString& patientUid);
    void showPersonalProgram(const QString& uidPPAssigned);
    void hideAllWidgets();

    void saveSplitterPosition();
    void restoreSplitterPosition();

    QModelIndex selectedIndex() const;

    QStandardItemModel* m_model {nullptr};

    QMap<QString, PatientProgramWidget*> m_wgts; ///< Виджеты ИП, назначенных для пациентов
};

#endif // PERSONALPROGRAMWIDGET_H
