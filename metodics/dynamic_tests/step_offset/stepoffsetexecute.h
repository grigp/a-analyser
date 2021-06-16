#ifndef STEPOFFSETEXECUTE_H
#define STEPOFFSETEXECUTE_H

#include <QWidget>

#include "stabdynamictestexecute.h"
#include "baseutils.h"

namespace Ui {
class StepOffsetExecute;
}

class SetMaxForceDialog;

class StepOffsetExecute : public StabDynamicTestExecute
{
    Q_OBJECT

public:
    explicit StepOffsetExecute(QWidget *parent = nullptr);
    ~StepOffsetExecute() override;

    void setParams(const QJsonObject &params) override;

protected:
    /*!
     * \brief Виртуальная функция, создающая окно пациента. Ложна быть перекрыта в подклассах
     */
    StabDynamicTestPatientWindow* createPatientWindow() override;

    /*!
     * \brief Действия по завершению теста
     */
    void finishTest() override;

    /*!
     * \brief Перекрытая функция заполнения панели специфичных элеменов управления для методики
     * \param frSpecific - указатель на фрейм
     */
    void fillSpecific(QFrame *frSpecific) override;

protected slots:
    void start() override;

    /*!
     * \brief Управляет началом / остановом / прерыванием записи
     */
    void recording() override;

    void getData(DeviceProtocols::DeviceData *data) override;

    void on_communicationError(const QString &drvName, const QString &port, const int errorCode) override;

private slots:
    void setMaxForceDialogAccepted();

private:
    Ui::StepOffsetExecute *ui;


    int m_stageTime {8};
    int m_repeatCount {4};
    int m_force {50};
    BaseUtils::Directions m_direction {BaseUtils::dirUp};

    SetMaxForceDialog* m_mfd {nullptr};
};

#endif // STEPOFFSETEXECUTE_H
