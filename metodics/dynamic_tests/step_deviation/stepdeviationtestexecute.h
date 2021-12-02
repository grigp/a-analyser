#ifndef STEPDEVIATIONTESTEXECUTE_H
#define STEPDEVIATIONTESTEXECUTE_H

#include "stabdynamictestexecute.h"
#include "baseutils.h"

#include <QObject>
#include <QWidget>

/*!
 * \brief Класс проведения теста со ступенчатым отклонением StepDeviationTestExecute class
 */
class StepDeviationTestExecute : public StabDynamicTestExecute
{
    Q_OBJECT
public:
    explicit StepDeviationTestExecute(QWidget *parent = nullptr);
    ~StepDeviationTestExecute() override;

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

    /*!
     * \brief Перекрытая функция, показывающая, что запись будет завершаться вручную
     * \return false - запись завершается вручную
     */
    bool isAutoFinishRecord() override {return false;}

protected slots:
    void start() override;

    /*!
     * \brief Управляет началом / остановом / прерыванием записи
     */
    void recording() override;

    void getData(DeviceProtocols::DeviceData *data) override;

    void on_communicationError(const QString &drvName, const QString &port, const int errorCode) override;


private:
    int m_maxTime {40};
    BaseUtils::Directions m_direction {BaseUtils::dirUp};

};

#endif // STEPDEVIATIONTESTEXECUTE_H
