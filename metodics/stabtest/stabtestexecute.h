#ifndef STABTESTEXECUTE_H
#define STABTESTEXECUTE_H

#include "stabtestparams.h"
#include "datadefines.h"
#include "deviceprotocols.h"

#include <QWidget>

namespace Ui {
class StabTestExecute;
}

class TestResultData;
class Stabilogram;
class Balistogram;
class Driver;
class PatientWindow;

/*!
 * \brief Виджет выполнения стабилометрического тестирования StabTestExecute class
 */
class StabTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit StabTestExecute(QWidget *parent = 0);
    ~StabTestExecute();

    void setParams(const QJsonObject &probeParams);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void start();

    void scaleChange(int scaleId);

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    /*!
     * \brief Центровка
     */
    void zeroing();

    /*!
     * \brief Калибровка
     */
    void calibrate();

    /*!
     * \brief Управляет началом / остановом / прерыванием записи
     */
    void recording();

    void showTrace(bool trace);

    void on_advChannelsClicked(bool checked);

    void splitterMoved(int pos,int index);

private:
    Ui::StabTestExecute *ui;

    /*!
     * \brief Возвращает параметры для текущей пробы.
     * Безопасная, не вызывает исключения
     */
    StabTestParams::ProbeParams probeParams();

    /*!
     * \brief Формирует список каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setChannels();

    void initRecSignals();

    /*!
     * \brief Действия по переходу к следующей пробе
     */
    void nextProbe();

    /*!
     * \brief Действия по завершению теста
     */
    void finishTest();

    /*!
     * \brief Показывает окно пациента
     * \param winCode - код окна
     */
    void showPatientWindow(const int winCode);
    /*!
     * \brief Прячет окно пациента
     */
    void hidePatientWindow();

    int m_freqStab = 50;        ///< Частота дискретизации стабилограммы
    int m_freqZ = 50;           ///< Частота дискретизации баллистограммы

    DataDefines::PatientKard m_kard;
    QList<StabTestParams::ProbeParams> m_params; ///< Параметры методики

    int m_probe = 0;                ///< Номер текущей пробы
    bool m_isRecording = false;     ///< Протекает ли запись данных
    int m_recCount = 0;             ///< Счетчик пакетов данных в пробе

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::StabControl* m_stabControl;  ///< Управление стабилографией в драйвере

    TestResultData *m_trd;  ///< Объект, записывающий данные в базу

//    bool m_patientWinPresent {false};
    PatientWindow* m_patientWin {nullptr};  ///< Окно пациента
};

#endif // STABTESTEXECUTE_H
