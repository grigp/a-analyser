#ifndef EVOLVENTAEXECUTE_H
#define EVOLVENTAEXECUTE_H

#include <QWidget>
#include <QLabel>

#include "datadefines.h"
#include "stabdynamictestexecute.h"
#include "deviceprotocols.h"
#include "baseutils.h"
#include "evolventadefines.h"

namespace Ui {
class EvolventaExecute;
}

class StabDynamicTestPatientWindow;
class EvolventaResultData;
class DecartCoordinatesSignal;

class EvolventaExecute : public StabDynamicTestExecute
{
    Q_OBJECT

public:
    explicit EvolventaExecute(QWidget *parent = nullptr);
    ~EvolventaExecute() override;

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

private:
    Ui::EvolventaExecute *ui;

    /*!
     * \brief Устанавливает цель, заданную полярными координатами
     * \param angle - угол
     * \param ampl - амплитуда
     */
    void setTargetPolar(const double angle, const double ampl);

    int m_speedMax {20};
    int m_radiusMax {40};
    int m_circleCount {4};
    BaseUtils::DirectionMode m_directionMode {BaseUtils::dmClockwise};

    EvolventaDefines::Stage m_stage {EvolventaDefines::stgUpwinding};

    ///< Координаты цели
    double m_tx {0};
    double m_ty {0};

    double m_targetAngle {0};    ///< Угол при анимировании эвольвенты
    double m_targetAmpl {0};     ///< Амплитуда при анимировании эвольвенты
    double m_endStageCount {0};  ///< Значение m_targetAngle при окончании этапа круга

    EvolventaResultData *m_res {nullptr};
    DecartCoordinatesSignal *m_targetSign {nullptr};

    QLabel *m_lblStage {nullptr};
};

#endif // EVOLVENTAEXECUTE_H
