#ifndef PULSEWIDGET_H
#define PULSEWIDGET_H

#include <QWidget>

#include "signalwidget.h"

namespace Ui {
class PulseWidget;
}

/*!
 * \brief Класс виджета - пульсометрии PulseWidget class
 */
class PulseWidget : public SignalWidget
{
    Q_OBJECT

public:
    explicit PulseWidget(Driver *drv, const QString channelId, QWidget *parent = 0);
    ~PulseWidget();

    /*!
     * \brief Начало новой пробы
     */
    void newProbe() override;

    /*!
     * \brief Прерывание пробы
     */
    void abortProbe() override;

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
    void on_resetPulse();

private:
    Ui::PulseWidget *ui;

    double m_pulseMiddle {0};
    int m_pulseCount {0};
};

#endif // PULSEWIDGET_H