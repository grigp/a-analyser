#ifndef BREATHWIDGET_H
#define BREATHWIDGET_H

#include <QWidget>
#include <QPushButton>

#include "signalwidget.h"

namespace Ui {
class BreathWidget;
}

class BreathSignal;

/*!
 * \brief Класс виджета - дыхания BreathWidget class
 */
class BreathWidget : public SignalWidget
{
    Q_OBJECT

public:
    explicit BreathWidget(Driver *drv, const QString channelId, QWidget *parent = 0);
    ~BreathWidget();

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
    void on_calibrate();

    void on_scaleChange(int scaleIdx);

    void on_recChange(bool checked);

private:
    Ui::BreathWidget *ui;

    void setRecordedChannels();

    void setRecButton(QPushButton *btn, const bool checked);

    DeviceProtocols::TensoControl* m_breathControl {nullptr};
    double m_min {-10};
    double m_max {10};

    BreathSignal* m_signal {nullptr};
};

#endif // BREATHWIDGET_H
