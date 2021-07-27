#ifndef OCTAEDRONEXECUTE_H
#define OCTAEDRONEXECUTE_H

#include <QWidget>

#include "signalsdefines.h"
#include "datadefines.h"
#include "stabdynamictestexecute.h"
#include "deviceprotocols.h"
#include "baseutils.h"

namespace Ui {
class OctaedronExecute;
}

class OctaedronPatientWindow;

class OctaedronExecute : public StabDynamicTestExecute
{
    Q_OBJECT

public:
    explicit OctaedronExecute(QWidget *parent = nullptr);
    ~OctaedronExecute() override;

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
    Ui::OctaedronExecute *ui;

    void setTargetsCoordinates();

    void createMarkerAndTargets();

    BaseUtils::CirceRoundRuleMode m_circeRoundRuleMode {BaseUtils::crmRadial};
    BaseUtils::DirectionMode m_directionMode {BaseUtils::dmCounterClockwise};
    int m_stageTime {5};
    int m_radius {50};

    OctaedronPatientWindow *m_patientWin {nullptr};

    QList<SignalsDefines::StabRec> m_targets; ///< Список целей
    int m_stage {-1};                         ///< Этап пробы. Совпадает с номером подсвеченной цели. Если -1, то для радиального - возврат в центр
    QSet<int> m_passed;                       ///< Множество пройденных этапов. Только для случайной последовательности при радиальном проходе
};

#endif // OCTAEDRONEXECUTE_H
