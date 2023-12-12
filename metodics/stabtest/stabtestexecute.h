#ifndef STABTESTEXECUTE_H
#define STABTESTEXECUTE_H

#include "stabtestparams.h"
#include "datadefines.h"
#include "deviceprotocols.h"
#include "metodicdefines.h"

#include <QWidget>

namespace Ui {
class StabTestExecute;
}

class TestResultData;
class Stabilogram;
class Balistogram;
class Driver;
class PatientWindow;
class BilateralResultData;

/*!
 * \brief Виджет выполнения стабилометрического тестирования StabTestExecute class
 */
class StabTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit StabTestExecute(QWidget *parent = nullptr);
    ~StabTestExecute() override;

    void setParams(const QJsonObject &probeParams);

protected:
    void closeEvent(QCloseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void start();

    void scaleChange(int scaleId);

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);
    void on_error(const int errorCode);

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

    void on_clearTrace();

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
     * \brief Добавляет канал билатеральных данных в пробу
     */
    void addBilatChannel();

    /*!
     * \brief Показывает окно пациента
     * \param winCode - код окна
     */
    void showPatientWindow(const int winCode);
    /*!
     * \brief Прячет окно пациента
     */
    void hidePatientWindow();

    /*!
     * \brief Возвращает строку с текстом ожидания события
     * \param eventName - название события
     * \param sec - время до центро
     */
    QString msgWaitEvent(const QString& eventName, const int sec) const;

    /*!
     * \brief Функция, помещающая комментарий поверх окна
     * \param comment - текст комментария
     */
    void setFrontComment(const QString& comment);

    int m_freqStab {50};        ///< Частота дискретизации стабилограммы
    int m_freqZ {50};           ///< Частота дискретизации баллистограммы
    int m_maxDiap {128};        ///< Максимальный диапазон

    DataDefines::PatientKard m_kard;
    QList<StabTestParams::ProbeParams> m_params; ///< Параметры методики

    int m_probe = 0;                ///< Номер текущей пробы
    bool m_isRecording = false;     ///< Протекает ли запись данных
    int m_recCount = 0;             ///< Счетчик пакетов данных в пробе

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::StabControl* m_stabControl {nullptr};             ///< Управление стабилографией в драйвере

    TestResultData *m_trd;  ///< Объект, записывающий данные в базу

//    bool m_patientWinPresent {false};
    PatientWindow* m_patientWin {nullptr};  ///< Окно пациента

    DeviceProtocols::MultiPlatformControl* m_bilatControl {nullptr};   ///< Управление билатаральным режимом в драйвере
    QRect m_platform1 {QRect(0, 0, 0, 0)};   ///< Платформы в билатеральном режиме
    QRect m_platform2 {QRect(0, 0, 0, 0)};

    int m_autoModeTimerId {-1};   ///< id таймера для режима автономной работы
    int m_stageNum {0};           ///< Номер этапа в автоматическом режиме
    int m_autoModeSecCounter {0};  ///< Счетчик секунд в автоматическом режиме
    int m_autoTimeRun {5};         ///< Время задержки до операции
    int m_autoTimeLatent {2};      ///< Длительность латентного периода
    QList<QList<MetodicDefines::AutoModeStaticStages>> m_stages;  ///< Список этапов для автоматического режима для каждой пробы
};

#endif // STABTESTEXECUTE_H
