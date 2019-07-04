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
class Ballistogram;
class Driver;

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

private slots:
    void start();
    void signalTest();

    void scaleChange(int scaleId);

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const int errorCode);

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

private:
    Ui::StabTestExecute *ui;

    /*!
     * \brief Возвращает параметры для текущей пробы.
     * Безопасная, не вызывает исключения
     */
    StabTestParams::ProbeParams probeParams();

    void initRecSignals();

    /*!
     * \brief Действия по переходу к следующей пробе
     */
    void nextProbe();

    /*!
     * \brief Действия по завершению теста
     */
    void finishTest();

    DataDefines::PatientKard m_kard;
    QList<StabTestParams::ProbeParams> m_params; ///< Параметры методики

    int m_probe = 0;                ///< Номер текущей пробы
    bool m_isRecording = false;     ///< Протекает ли запись данных
    int m_recCount = 0;             ///< Счетчик пакетов данных в пробе

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::StabControl* m_stabControl;  ///< Управление стабилографией в драйвере

    TestResultData *m_trd;  ///< Объект, записывающий данные в базу
    Stabilogram *m_stb;     ///< Записываемая стабилограмма
    Ballistogram *m_z;      ///< Записываемая баллистограмма
};

#endif // STABTESTEXECUTE_H
