#ifndef DYNAMOWIDGET_H
#define DYNAMOWIDGET_H

#include <QWidget>
#include <QPushButton>

#include "signalwidget.h"

namespace Ui {
class DynamoWidget;
}

class DynamoSignal;

/*!
 * \brief Класс виджета - силомера DynamoWidget class
 */
class DynamoWidget : public SignalWidget
{
    Q_OBJECT

public:
    explicit DynamoWidget(Driver *drv, const QString channelId, QWidget *parent = 0);
    ~DynamoWidget();

    /*!
     * \brief Начало новой пробы
     */
    void newProbe() override;

    /*!
     * \brief Прерывание пробы
     */
    void abortProbe() override;

    /*!
     * \brief Сохранение пробы
     */
    void saveProbe() override;

    /*!
     * \brief Получение данных от устройства
     * \param data - данные
     */
    void getData(DeviceProtocols::DeviceData *data) override;

    void record(DeviceProtocols::DeviceData *data) override;

    void setFrequency(const int frequency) override;

    /*!
     * \brief Управляет допустимостью элементов на панелях
     */
    void enabledControls(const bool enabled) override;

    /*!
     * \brief Устанавливает канал, записываемый всегда
     * \param channelId - идентификатор канала
     */
    void setAllwaysRecordingChannel(const QString &channelId) override;

private slots:
    void on_resetValueMax();

    void on_calibrate();

    void on_scaleChange(int scaleIdx);

    void on_dynRecChange(bool checked);

private:
    Ui::DynamoWidget *ui;

    void setRecordedChannels();

    void setRecButton(QPushButton *btn, const bool checked);

    DeviceProtocols::TensoControl* m_dynControl {nullptr};
    double m_min {0};
    double m_max {10};
    double m_valueMax {0};

    DynamoSignal* m_dynamo {nullptr};

};

#endif // DYNAMOWIDGET_H
