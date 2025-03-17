#ifndef MYOGRAMWIDGET_H
#define MYOGRAMWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

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
    void on_started();

    /*!
     * \brief Изменение режима записи всех отведений миограмм
     */
    void on_recMyoClick(bool checked);

    /*!
     * \brief Изменение режима записи одного из отведений миограмм
     */
    void on_recMyoChanClick(bool checked);

    void on_myoScaleChange(int scaleIdx);

    void on_resetValuesChan1();
    void on_resetValuesChan2();
    void on_resetValuesChan3();
    void on_resetValuesChan4();


private:
    Ui::MyogramWidget *ui;

    void setSubChannelsRecButtons();

    void setRecordedChannels();

    void setRecButton(QPushButton *btn, const bool checked);

    /*!
     * \brief Возвращает true, если подканал с номерм scn будет записываться
     */
    bool isSubChanRecord(const int scn) const;

    int subChanCount() const;

    void setIndicatorsStyle();
    void showChannelValues(const int idx,
                           QLabel* lblValue,
                           QLabel* lblValueCur,
                           QLabel* lblDecrease,
                           QLabel* lblMin,
                           QLabel* lblMax,
                           QLabel* lblDiap);

    DeviceProtocols::MyoControl* m_myoControl {nullptr};

    QList<QPushButton*> m_btnSubChans;  ///< Указатели на кнопки записи подканалов

    double m_valueDisplay[4] {0, 0, 0, 0};
    double m_valueMin[4] {10000, 10000, 10000, 10000};
    double m_valueMax[4] {-10000, -10000, -10000, -10000};
    Myogram *m_myo {nullptr};
    double m_amplMyo {2};
    QList<QPushButton*> m_subChanels;  ///< Список кнопок, управления записью подканалов 0, 1, ...
};

#endif // MYOGRAMWIDGET_H
