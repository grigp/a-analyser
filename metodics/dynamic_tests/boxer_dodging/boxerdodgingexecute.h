#ifndef BOXERDODGINGEXECUTE_H
#define BOXERDODGINGEXECUTE_H

#include <QObject>
#include <QWidget>
#include "stabdynamictestexecute.h"


class BoxerDodgingPatientWindow;

/*!
 * \brief Класс проведения теста тренажера с уклонением для боксеров BoxerDodgingExecute class
 */
class BoxerDodgingExecute : public StabDynamicTestExecute
{
    Q_OBJECT
public:
    explicit BoxerDodgingExecute(QWidget *parent = nullptr);
    ~BoxerDodgingExecute() override;

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
    void createMarker();

    /*!
     * \brief Создает и показывает окно пациента
     */
    void createAndShowPatientWindow();

    /*!
     * \brief Прячет окно пациента
     */
    void hidePatientWindow();

    BoxerDodgingPatientWindow* m_patientWin {nullptr};
};

#endif // BOXERDODGINGEXECUTE_H
