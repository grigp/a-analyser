#ifndef TRENTAKEPUTEXECUTE_H
#define TRENTAKEPUTEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>

#include "datadefines.h"
#include "deviceprotocols.h"
#include "trentakeputdefines.h"

namespace Ui {
class TrenTakePutExecute;
}

class Driver;

/*!
 * \brief Класс виджета проведения сеанса тренинга TrenagerExecute class
 */
class TrenTakePutExecute : public QWidget
{
    Q_OBJECT

public:
    explicit TrenTakePutExecute(QWidget *parent = 0);
    ~TrenTakePutExecute();

    void setParams(const QJsonObject &probeParams);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void start();

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    void on_selectChannel(int chanIdx);

private:
    Ui::TrenTakePutExecute *ui;

    void setZones(const QJsonArray &arrZones, QList<TrenTakePutDefines::GameZoneInfo> &zones);
    void setElements(const QJsonArray &arrElements, QList<TrenTakePutDefines::GameElementInfo> &elements);

    /*!
     * \brief Формирует список каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setChannels();

    int m_freqStab = 50;        ///< Частота дискретизации стабилограммы
    int m_freqZ = 50;           ///< Частота дискретизации баллистограммы

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::StabControl* m_stabControl;  ///< Управление стабилографией в драйвере

    TrenTakePutDefines::TakeOrder m_TakeTakeOrder {TrenTakePutDefines::toEnabledPrimary};
    TrenTakePutDefines::TakeOrder m_PutTakeOrder {TrenTakePutDefines::toEnabledPrimary};
    TrenTakePutDefines::StageMode m_stageMode {TrenTakePutDefines::smTakePut};

    ///< Зоны игры и элементы
    QList<TrenTakePutDefines::GameZoneInfo> m_zonesTake;
    QList<TrenTakePutDefines::GameZoneInfo> m_zonesPut;
    QList<TrenTakePutDefines::GameElementInfo> m_elementsTake;
    QList<TrenTakePutDefines::GameElementInfo> m_elementsPut;


//    DataDefines::PatientKard m_kard;

};

#endif // TRENTAKEPUTEXECUTE_H
