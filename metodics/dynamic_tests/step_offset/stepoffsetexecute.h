#ifndef STEPOFFSETEXECUTE_H
#define STEPOFFSETEXECUTE_H

#include <QWidget>
#include <QLabel>

#include "stabdynamictestexecute.h"
#include "basedefines.h"
#include "stepoffsetdefines.h"

namespace Ui {
class StepOffsetExecute;
}

class SetMaxForceDialog;
class StepOffsetResultData;
class StepOffsettPatientWindow;

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

    void setTargetCoordinates();

    int m_stageTime {8};
    int m_repeatCount {4};
    int m_forcePercent {50};
    BaseDefines::Directions m_direction {BaseDefines::dirUp};
    int m_force {0};

    SetMaxForceDialog* m_mfd {nullptr};
    StepOffsetResultData* m_res {nullptr};

    StepOffsetDefines::Stage m_stage {StepOffsetDefines::stgWaiting};  //! Этап теста
    int m_stageCounter {0};                                            //! Счетчик пакетов на этапе
    int m_recordCounter {0};                                           //! Счетчик пакетов записи
    int m_lastCompensation {0};                                        //! Отсчет начала последнего участка компенсации
    int m_repeatCounter {0};                                           //! Счетчик кол-ва повторений

    ///< Координаты цели
    double m_tx {0};
    double m_ty {0};

    QLabel* m_lblRemain;
    StepOffsettPatientWindow* m_patientWin {nullptr};
};

#endif // STEPOFFSETEXECUTE_H
