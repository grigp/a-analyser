#ifndef BEDSIDESCALESTESTEREXECUTE_H
#define BEDSIDESCALESTESTEREXECUTE_H

#include <QWidget>
#include <QJsonObject>

#include "datadefines.h"
#include "deviceprotocols.h"
#include "bedsidescalesdefines.h"

namespace Ui {
class BedsideScalesTesterExecute;
}

class TestResultData;
class Driver;
class WeightPlateSignal;
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
    BedsideScalesDefines::Mode m_mode {BedsideScalesDefines::bsmTester};

    bool m_isRecording {false};
    WeightPlateSignal *m_wps {nullptr};     ///< Записываемый сигнал
    Balistogram *m_z {nullptr};

};

#endif // BEDSIDESCALESTESTEREXECUTE_H
