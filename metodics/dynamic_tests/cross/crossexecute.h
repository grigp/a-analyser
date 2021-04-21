#ifndef CROSSEXECUTE_H
#define CROSSEXECUTE_H

#include <QWidget>
#include <QJsonObject>

#include "datadefines.h"
#include "crossdefines.h"
#include "stabdynamictestexecute.h"
#include "deviceprotocols.h"

namespace Ui {
class CrossExecute;
}

/*!
 * \brief Виджет выполнения теста на устойчивость CrossExecute class
 */
class CrossExecute : public StabDynamicTestExecute
{
    Q_OBJECT

public:
    explicit CrossExecute(QWidget *parent = nullptr);
    ~CrossExecute() override;

    void setParams(const QJsonObject &params) override;

protected:

protected slots:
    void start() override;


private slots:

//    void scaleChange(int scaleId);

//    void getData(DeviceProtocols::DeviceData *data);
//    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

//    /*!
//     * \brief Центровка
//     */
//    void zeroing();

//    /*!
//     * \brief Калибровка
//     */
//    void calibrate();

//    /*!
//     * \brief Управляет началом / остановом / прерыванием записи
//     */
//    void recording();

//    void on_advChannelsClicked(bool checked);

//    void splitterMoved(int pos,int index);

private:
    Ui::CrossExecute *ui;

    int m_stageTime {10};
    int m_repeatCount {1};
    int m_centerSize {15};
    CrossDefines::ChangeStateMode m_changeStateMode{CrossDefines::csmReturn};
    CrossDefines::DirectionMode m_directionMode{CrossDefines::dmRandom};

//    int m_freqStab = 50;        ///< Частота дискретизации стабилограммы
//    int m_freqZ = 50;           ///< Частота дискретизации баллистограммы

//    DataDefines::PatientKard m_kard;

//    bool m_isRecording = false;     ///< Протекает ли запись данных
//    int m_recCount = 0;             ///< Счетчик пакетов данных в пробе

//    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
//    DeviceProtocols::StabControl* m_stabControl;  ///< Управление стабилографией в драйвере

//    TestResultData *m_trd;  ///< Объект, записывающий данные в базу
};

#endif // CROSSEXECUTE_H
