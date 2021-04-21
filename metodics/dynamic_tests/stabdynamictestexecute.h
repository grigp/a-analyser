#ifndef STABDYNAMICTESTEXECUTE_H
#define STABDYNAMICTESTEXECUTE_H

#include <QWidget>
#include <QJsonObject>

#include "datadefines.h"
#include "crossdefines.h"
#include "deviceprotocols.h"

namespace Ui {
class StabDynamicTestExecute;
}

class TestResultData;
class Stabilogram;
class Ballistogram;
class Driver;

/*!
 * \brief Базовый класс виджета выполнения стабилометрического динамического теста StabDynamicTestExecute class
 */
class StabDynamicTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit StabDynamicTestExecute(QWidget *parent = nullptr);
    ~StabDynamicTestExecute() override;

    virtual void setParams(const QJsonObject &params);

protected:
    void closeEvent(QCloseEvent *event) override;

    /*!
     * \brief Устанавливает заголовок окна
     */
    void setTitle(const QString &title);

    TestResultData *trd() {return m_trd;}

    DeviceProtocols::StabControl* stabControl() {return m_stabControl;}

    Driver* driver() {return m_driver;}

    bool isRecording() {return m_isRecording;}
    int recCount() {return m_recCount;}
    int freqStab() {return m_freqStab;}
    int freqZ() {return m_freqZ;}

    void isShowValues(const bool isShow);
    void isTraceControl(const bool isTrace);

protected slots:
    virtual void start();

    virtual void getData(DeviceProtocols::DeviceData *data);
    virtual void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    /*!
     * \brief Управляет началом / остановом / прерыванием записи
     */
    virtual void recording();

private slots:

    void scaleChange(int scaleId);


    /*!
     * \brief Центровка
     */
    void zeroing();

    /*!
     * \brief Калибровка
     */
    void calibrate();

    void on_advChannelsClicked(bool checked);

    void splitterMoved(int pos,int index);

private:
    Ui::StabDynamicTestExecute *ui;

    /*!
     * \brief Формирует список каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setChannels();

    int m_freqStab {50};        ///< Частота дискретизации стабилограммы
    int m_freqZ {50};           ///< Частота дискретизации баллистограммы

    DataDefines::PatientKard m_kard;

    bool m_isRecording {false};     ///< Протекает ли запись данных
    int m_recCount {0};             ///< Счетчик пакетов данных в пробе

    Driver* m_driver {nullptr};                             ///< Драйвер передающий данные
    DeviceProtocols::StabControl* m_stabControl {nullptr};  ///< Управление стабилографией в драйвере

    TestResultData *m_trd;  ///< Объект, записывающий данные в базу
};

#endif // STABDYNAMICTESTEXECUTE_H
