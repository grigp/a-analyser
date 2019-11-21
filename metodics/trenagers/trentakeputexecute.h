#ifndef TRENTAKEPUTEXECUTE_H
#define TRENTAKEPUTEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>

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

    void resizeEvent(QResizeEvent* event);

private slots:
    void start();

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    void on_selectChannel(int chanIdx);
    void on_zeroing();

private:
    Ui::TrenTakePutExecute *ui;

    void setZones(const QJsonArray &arrZones, QList<TrenTakePutDefines::GameZoneInfo> &zones);
    void setElements(const QJsonArray &arrElements, QList<TrenTakePutDefines::GameElementInfo> &elements);
    void setMarker(const QJsonObject &objMarker);

    /*!
     * \brief Формирует список каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setChannels();

    /*!
     * \brief Генерация новой сцены
     */
    void newScene();

    void allocBySeparatePositions(TrenTakePutDefines::TakeOrder &takeOrder,
                                  QList<TrenTakePutDefines::GameZoneInfo> &zones,
                                  QList<TrenTakePutDefines::GameElementInfo> &elements);

    void allocElements(QList<TrenTakePutDefines::GameZoneInfo> &zones,
                       QList<TrenTakePutDefines::GameElementInfo> &elements,
                       int enabled);   ///< -1 - все, 1 - только enabled == true, 0 - только enabled == false
    bool isEmptyZonesPresent(QList<TrenTakePutDefines::GameZoneInfo> &zones) const;

    QGraphicsScene* m_scene {nullptr};
    double m_prop = 1; ///< Пропорция для пересчера базовой сцены 2000 x 2000 в реальные размеры игровой сцены
    QGraphicsPixmapItem *m_marker {nullptr};

    int m_frequency = 50;        ///< Частота дискретизации

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::DecartCoordControl* m_dcControl;  ///< Управление декартовыми данными в драйвере

    TrenTakePutDefines::TakeOrder m_takeTakeOrder {TrenTakePutDefines::toEnabledPrimary};
    TrenTakePutDefines::TakeOrder m_putTakeOrder {TrenTakePutDefines::toEnabledPrimary};
    TrenTakePutDefines::StageMode m_stageMode {TrenTakePutDefines::smTakePut};

    ///< Зоны игры и элементы
    QList<TrenTakePutDefines::GameZoneInfo> m_zonesTake;
    QList<TrenTakePutDefines::GameZoneInfo> m_zonesPut;
    QList<TrenTakePutDefines::GameElementInfo> m_elementsTake;
    QList<TrenTakePutDefines::GameElementInfo> m_elementsPut;


//    DataDefines::PatientKard m_kard;

};

#endif // TRENTAKEPUTEXECUTE_H
