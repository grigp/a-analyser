#ifndef PULSEWIDGET_H
#define PULSEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>


#include "signalwidget.h"

namespace Ui {
class PulseWidget;
}

class Ritmogram;

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

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_resetPulse();
    void on_pulseRecChange(bool checked);

    void on_delAtfMode(bool value);
    void on_changeLoBound(int value);
    void on_changeUpBound(int value);

private:
    Ui::PulseWidget *ui;

    void setRecordedChannels();

    void setRecButton(QPushButton *btn, const bool checked);
    void setDelAtf(const bool isDelete);

    double m_pulseMiddle {0};
    int m_pulseCount {0};
    bool m_isFinish {false};

    int m_pickTimerId {-1};

    Ritmogram* m_ritmogram {nullptr};
};

#endif // PULSEWIDGET_H
