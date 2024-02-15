#ifndef BOXERDODGINGEXECUTE_H
#define BOXERDODGINGEXECUTE_H

#include <QObject>
#include <QWidget>
#include "stabdynamictestexecute.h"

#include "boxerdodgingdefines.h"


class BoxerDodgingPatientWindow;
class BoxerDodgingResultData;

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

    /*!
     * \brief Функция, помещающая комментарий поверх окна
     * \param comment - текст комментария
     */
    void setFrontComment(const QString& comment, const bool isPatientOnly = false, const QString& styleSheet = "") override;

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

    /*!
     * \brief Переход на следующий этап
     */
    void nextStage(const bool isStart);

    int m_deviationThreshold {30};
    int m_time {180};
    int m_stimulTimeMin {3};
    int m_stimulTimeMax {10};

    BoxerDodgingPatientWindow* m_patientWin {nullptr};                     ///< Окно пациента
    BoxerDodgingDefines::Stages m_stage {BoxerDodgingDefines::bdsBase};    ///< Этап

    int m_stageCounter {0};            ///< Счетчик пакетов на этапе
    int m_nextStageCount {0};          ///< Кол-во пакетов для этапа

    BoxerDodgingResultData * m_res {nullptr};
};

#endif // BOXERDODGINGEXECUTE_H
