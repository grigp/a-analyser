#ifndef MYOGRAMWIDGET_H
#define MYOGRAMWIDGET_H

#include <QWidget>
#include <QPushButton>

#include "signalwidget.h"

namespace Ui {
class MyogramWidget;
}

class Myogram;

/*!
 * \brief Класс виджета - миограммы MyogramWidget class
 */
class MyogramWidget : public SignalWidget
{
    Q_OBJECT

public:
    explicit MyogramWidget(Driver *drv, const QString channelId, QWidget *parent = 0);
    ~MyogramWidget();

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
    /*!
     * \brief Изменение режима записи всех отведений миограмм
     */
    void on_recMyoClick(bool checked);

    /*!
     * \brief Изменение режима записи одного из отведений миограмм
     */
    void on_recMyoChanClick(bool checked);

    void on_myoScaleChange(int scaleIdx);


private:
    Ui::MyogramWidget *ui;

    void setSubChannelsRecButtons();

    void setRecordedChannels();

    void setRecButton(QPushButton *btn, const bool checked);

    DeviceProtocols::MyoControl* m_myoControl {nullptr};

    QList<QPushButton*> m_btnSubChans;  ///< Указатели на кнопки записи подканалов

    Myogram *m_myo;
    double m_amplMyo {2};
};

#endif // MYOGRAMWIDGET_H