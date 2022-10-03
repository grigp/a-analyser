#ifndef BEDSIDESCALESTESTEREXECUTE_H
#define BEDSIDESCALESTESTEREXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QQueue>
#include <QMediaPlayer>

#include "datadefines.h"
#include "deviceprotocols.h"
#include "bedsidescalesdefines.h"

namespace Ui {
class BedsideScalesTesterExecute;
}

class TestResultData;
class Driver;
class WeightPlateSignal;
class WeighingResultData;
class Balistogram;

/*!
 * \brief Виджет выполнения методики тестирования прикроватных весов The BedsideScalesTesterExecute class
 */
class BedsideScalesTesterExecute : public QWidget
{
    Q_OBJECT

public:
    explicit BedsideScalesTesterExecute(QWidget *parent = nullptr);
    ~BedsideScalesTesterExecute();

    void setParams(const QJsonObject &probeParams);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void start();

    void on_scaleChange(int scaleId);
    void on_zeroing();

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    /*!
     * \brief Калибровка
     */
    void calibrate();

    /*!
     * \brief Управляет началом / остановом / прерыванием записи
     */
    void recording();

    void on_selectItem(const int idx);

private:
    Ui::BedsideScalesTesterExecute *ui;

    void setupUI();

    void getDataWeight(DeviceProtocols::DeviceData *data);
    void getDataADC(DeviceProtocols::DeviceData *data);

    DataDefines::PatientKard m_kard;
    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::TensoControl* m_tenzoControl;  ///< Управление силовыми каналами в драйвере

    TestResultData *m_trd;  ///< Объект, записывающий данные в базу

    int m_freq {150};
    int m_counter {0};                      ///< Счетчик принятых пакетов данных
    int m_recCounter {0};                   ///< Счетчик записанных пакетов данных
    BedsideScalesDefines::Mode m_mode {BedsideScalesDefines::bsmTester};
    bool m_isAutoRecord {false};
    QTime m_startTime {QTime::fromString("0:00:20", "h:mm:ss")};
    QTime m_recLength {QTime::fromString("0:10:00", "h:mm:ss")};
    bool m_isSound {true};
    bool m_isIntervalScaling {true};
    QTime m_scalingInterval = {QTime::fromString("05:00", "mm:ss")};
    int m_averageTime {3};

    bool m_isRecording {false};
    bool m_isCalibrated {false};
    WeightPlateSignal *m_wps {nullptr};     ///< Записываемый сигнал
    Balistogram *m_weight {nullptr};
    WeighingResultData *m_wrd {nullptr};

    QQueue<QVector<double>> m_values;       ///< Значения за последние N секунд
    QVector<double> m_offsets;              ///< Смещения (центровка)

    int m_curScaleId {0};                   ///< Текущий индекс масштаба
    QMediaPlayer m_player;

    enum ParticalWeightingMode
    {
        pwmWait = 0
      , pwmAveraging
    };
    ParticalWeightingMode m_pwm {pwmWait};
    int m_scalingIntervalPt {0};
    int m_averageTimePt {0};
    int m_pwmCounter {0};
    int m_partWeightCount {0};
    double m_pwmWeight {0};
};

#endif // BEDSIDESCALESTESTEREXECUTE_H
