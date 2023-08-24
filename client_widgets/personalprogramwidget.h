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

protected:
    void timerEvent(QTimerEvent *event) override;

private slots:
    void on_splitterMoved(int,int);

    void on_run();
    void on_delete();
    void on_params();

    void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void selectPatient(const QModelIndex index);

    void on_selectPatient(const QString &patientUid);

    void on_newTest(const QString &testUid);

    /*!
     * \brief Обработка сигналов о назначении индивидуальной программы для пациента
     * \param patientUid - uid пациента
     * \param ppUid - uid индивидуальной программы
     */
    void on_assignPPForPatient(const QString& patientUid, const QString& ppUid);

    /*!
     * \brief Обработка сигналов о снятии индивидуальной программы для пациента
     * \param patientUid - uid пациента
     */
    void on_cancelPPForPatient(const QString& patientUid);

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

    void doRunTest();

    QDateTime getDateTimeByString(const QString& s) const;

    QJsonObject findEmptyTestInfo(const QJsonArray& arr);

    /*!
     * \brief Возвращает объект - описатель следующего теста по ИП
     * \param objPPAll - описание индивидуальной программы
     * \param objTest - JSonObject с описанием теста, если тест есть и его можно сейчас выполнить. QJsonObject(), если теста нет или его нельзя выполнить
     * \return true, если программа еще работает, false, если должна быть завершена
     */
    bool getNextTestInfo(const QJsonObject& objPPAll, QJsonObject& objTest);

    /*!
     * \brief Запускает следующий тест по индивидуальной программе
     * \param objTest - объект с описанием теста, который необходимо выполнить
     */
    void runTest(const QJsonObject& objTest);

    /*!
     * \brief Записывает в индивидуальную программу данные о проведенном тесте
     */
    void appendTestCompletionInfoToPP();

    QStandardItemModel* m_model {nullptr};

    QMap<QString, PatientProgramWidget*> m_wgts; ///< Виджеты ИП, назначенных для пациентов

    QJsonObject m_objPPExecuted {QJsonObject()};  ///< Выполняемая в настоящий момент индивидуальная программа
    QString m_activePatientUid {""};              ///< uid пациента, для которого запущена на выполнение ДП
    QString m_activeMethodicUid {""};             ///< uid текущей методики из запущеной на выполнение ДП
    QString m_finishedTestUid {""};               ///< uid теста, только что проведенного в рамках ДП
    int m_currentDP {-1};                         ///< Номер текущей дневной программы на время выполнения теста
    int m_currentTest {-1};                       ///< Номер текущего теста на время выполнения теста
    int m_tmNextStep {-1};                        ///< таймер задержки перед запуском следующей методики
};

#endif // PERSONALPROGRAMWIDGET_H
