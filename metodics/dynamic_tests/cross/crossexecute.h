#ifndef CROSSEXECUTE_H
#define CROSSEXECUTE_H

#include <QWidget>
#include <QJsonObject>

#include "datadefines.h"
#include "crossdefines.h"
#include "stabdynamictestexecute.h"
#include "deviceprotocols.h"

namespace Ui {
class CrossExecute;
}

class StabDynamicTestPatientWindow;

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

    int m_stageTime {10};
    int m_repeatCount {1};
    int m_centerSize {15};
    int m_delayTime {5};
    CrossDefines::ChangeStateMode m_changeStateMode{CrossDefines::csmReturn};
    CrossDefines::DirectionMode m_directionMode{CrossDefines::dmRandom};

    //! Прохождение этапов теста. Значение - кол-во повторений на каждом этапе. 0 -> m_repeatCount.
    //! Индексы CrossDefines::dirUp .. CrossDefines::dirLeft
    int m_stagesProcess[4] {0};

    //! Текущее направление перемещения
    CrossDefines::Directions m_curDirection {CrossDefines::dirNone};
};

#endif // CROSSEXECUTE_H
