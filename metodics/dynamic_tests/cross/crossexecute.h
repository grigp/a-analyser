#ifndef CROSSEXECUTE_H
#define CROSSEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QLabel>

#include "datadefines.h"
#include "crossdefines.h"
#include "stabdynamictestexecute.h"
#include "deviceprotocols.h"
#include "baseutils.h"

namespace Ui {
class CrossExecute;
}

class StabDynamicTestPatientWindow;
class CrossResultData;

/*!
 * \brief Виджет выполнения теста на устойчивость CrossExecute class
 */
class CrossExecute : public StabDynamicTestExecute
{
    Q_OBJECT

public:
    explicit CrossExecute(QWidget *parent = nullptr);
    ~CrossExecute() override;

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
    Ui::CrossExecute *ui;

    /*!
     * \brief Возвращает направление прохода в зависимости от этапа теста
     * \return true, если движение определено или false, если тест окончен
     * m_curDirection - текущее направление;
     */
    bool newDirection();

    /*!
     * \brief Проверяет, завершен ли тест
     * Все значения массива m_stagesProcess[] должны равняться m_repeatCount
     */
    bool isTestFinished();

    /*!
     * \brief Генерирует новые координаты цели
     * \param tx, ty - новые координаты
     * \return true, если не достигли края
     */
    bool newCoordinatesTarget(double &tx, double &ty);

    /*!
     * \brief Вызывается на этапе ожидания и возвращает true, если цель можно двигать в следущем направлении
     */
    bool waitingSuccessful();

    QString stagesRemained(const BaseUtils::Directions dir) const;

    ///< Координаты цели
    double m_tx {0};
    double m_ty {0};

    int m_stageTime {10};
    int m_repeatCount {1};
    int m_centerSize {15};
    int m_delayTime {5};
    CrossDefines::ChangeStateMode m_changeStateMode{CrossDefines::csmReturn};
    BaseUtils::DirectionMode m_directionMode{BaseUtils::dmRandom};

    //! Прохождение этапов теста. Значение - кол-во повторений на каждом этапе. 0 -> m_repeatCount.
    //! Индексы CrossDefines::dirUp .. CrossDefines::dirLeft
    int m_stagesProcess[4] {0};

    int m_waitCounter {0};   ///< Счетчик в режиме ожидания

    QLabel* m_lblUp;
    QLabel* m_lblDn;
    QLabel* m_lblRt;
    QLabel* m_lblLf;

    //! Текущее направление перемещения
    BaseUtils::Directions m_curDirection {BaseUtils::dirNone};

    //! Текущий этап (переход / ожидание)
    CrossDefines::Stage m_stage {CrossDefines::stgNo};

    CrossResultData* m_res {nullptr};
};

#endif // CROSSEXECUTE_H
