#ifndef TRIANGLEEXECUTE_H
#define TRIANGLEEXECUTE_H

#include <QObject>

#include "stabdynamictestexecute.h"
#include "baseutils.h"

/*!
 * \brief Класс проведения теста "Треугольник" TriangleExecute class
 */
class TriangleExecute : public StabDynamicTestExecute
{
    Q_OBJECT
public:
    explicit TriangleExecute(QWidget *parent = nullptr);
    ~TriangleExecute() override;

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
    BaseUtils::DirectionMode m_directionMode {BaseUtils::dmClockwise};
    int m_force {75};
    int m_stageTime {2};
    int m_minDeviation {50};
    bool m_showMarkerAnal {false};

};

#endif // TRIANGLEEXECUTE_H
