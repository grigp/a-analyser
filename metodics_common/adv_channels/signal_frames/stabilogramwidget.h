#ifndef STABILOGRAMWIDGET_H
#define STABILOGRAMWIDGET_H

#include <QWidget>
#include <QPushButton>

#include "signalwidget.h"

namespace Ui {
class StabilogramWidget;
}

class Stabilogram;
class Balistogram;


/*!
 * \brief Класс виджета - стабилограммы StabilogramWidget class
 */
class StabilogramWidget : public SignalWidget
{
    Q_OBJECT

public:
    explicit StabilogramWidget(Driver *driver, const QString channelId, QWidget *parent = 0);
    ~StabilogramWidget();

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

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_changeScaleStab(int scaleIdx);
    void on_changeScaleZ(int scaleIdx);

    void on_zeroingStab();
    void on_calibrate();

    void on_recStabClick(bool checked);
    void on_recZClick(bool checked);

private:
    Ui::StabilogramWidget *ui;

    void showAdvancedInfo(DeviceProtocols::StabDvcData *stabData);

    void setRecordedChannels();

    void setRecButton(QPushButton *btn, const bool checked);

    QVector<double> m_recStab;
    QVector<double> m_recZ;
    bool m_isShowMassa {false};
//    bool m_isRecStab {false};
//    bool m_isRecZ {false};

    Stabilogram *m_stb;     ///< Записываемая стабилограмма
    Balistogram *m_z;      ///< Записываемая баллистограмма
};

#endif // STABILOGRAMWIDGET_H
