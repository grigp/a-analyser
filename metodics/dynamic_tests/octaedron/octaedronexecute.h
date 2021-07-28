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

    /*!
     * \brief Генерирует номер первого этапа
     */
    void getFirstStage();

    /*!
     * \brief Генерирует номер следующего этапа
     * \return true, если есть следующий этап и false, если нет (пора завершать тест)
     */
    bool getNextStage();

    /*!
     * \brief Возвращает номер следующего этапа в случае случайного выбора
     */
    int getRandomStage();

    /*!
     * \brief Возвращает позицию цели в зависимости от текущего этапа
     */
    int getCurrentTargetPosition();

    /*!
     * \brief Устанавливает текущую цель
     */
    void setCurrentTarget();

    BaseUtils::CirceRoundRuleMode m_circeRoundRuleMode {BaseUtils::crmRadial};
    BaseUtils::DirectionMode m_directionMode {BaseUtils::dmCounterClockwise};
    int m_stageTime {5};
    int m_radius {50};

    OctaedronPatientWindow *m_patientWin {nullptr};

    QList<SignalsDefines::StabRec> m_targets; ///< Список целей
    int m_stage {-1};                         ///< Индекс в списке этапов пробы.
                                              ///< SequenceRadial.at(m_stage) - номер подсвеченной цели
                                              ///< Если -1, то для радиального - возврат в центр
    QSet<int> m_passed;                       ///< Множество пройденных этапов. Только для случайной последовательности при радиальном проходе
    int m_stageCounter {0};                   ///< Счетчик пакетов на этапе
};

#endif // OCTAEDRONEXECUTE_H
