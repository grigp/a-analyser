#ifndef OCTAEDRONEXECUTE_H
#define OCTAEDRONEXECUTE_H

#include <QWidget>

#include "signalsdefines.h"
#include "datadefines.h"
#include "stabdynamictestexecute.h"
#include "deviceprotocols.h"
#include "basedefines.h"

namespace Ui {
class OctaedronExecute;
}

class OctaedronPatientWindow;
class OctaedronResultData;

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

    void addStageToResult();

    /*!
     * \brief Возвращает координаты текущей цели
     * \param x и y - координаты
     * \param trgtPos - номер позиции цели
     */
    void getCurrentTargetCoords(int& trgtPos, double& x, double& y);

    /*!
     * \brief Возвращает расстояние от маркера до текущей цели
     */
    double distanceToTarget();

    /*!
     * \brief Возвращает true, усли этап закончен
     */
    bool isStageEnded();

    BaseDefines::CirceRoundRuleMode m_circeRoundRuleMode {BaseDefines::crmRadial};
    BaseDefines::DirectionMode m_directionMode {BaseDefines::dmCounterClockwise};
    int m_stageTime {5};
    int m_radius {50};
    BaseDefines::StageFinishMode m_stageFinishMode {BaseDefines::sfmFixedTime};
    int m_holdingTime {1};
    int m_holdingAmplitude {10};
    bool m_isShowOnlyCurrentPoint {false};


    OctaedronPatientWindow *m_patientWin {nullptr};

    QList<BaseDefines::TargetInfo> m_targets; ///< Список целей
    int m_stage {0};                          ///< Индекс в списке этапов пробы.
                                              ///< SequenceRadial.at(m_stage) - номер подсвеченной цели
                                              ///< Если -1, то для радиального - возврат в центр
    QSet<int> m_passed;                       ///< Множество пройденных этапов. Только для случайной последовательности при радиальном проходе
    int m_stageCounter {0};                   ///< Счетчик пакетов на этапе
    int m_holdingCouter {0};                  ///< Счетчик пакетов при удержании цели

    OctaedronResultData *m_res {nullptr};
};

#endif // OCTAEDRONEXECUTE_H
