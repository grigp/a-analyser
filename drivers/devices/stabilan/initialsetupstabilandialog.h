#ifndef INITIALSETUPSTABILANDIALOG_H
#define INITIALSETUPSTABILANDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QMediaPlayer>

#include "deviceprotocols.h"

namespace Ui {
class InitialSetupStabilanDialog;
}

/*!
 * \brief Класс диалога начальной настройки стабилоанализатора Стабилан-01 The InitialSetupStabilanDialog class
 */
class InitialSetupStabilanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitialSetupStabilanDialog(DeviceProtocols::Ports port,
                                        const QJsonObject &params,
                                        const QString &comment,
                                        QWidget *parent = nullptr);
    ~InitialSetupStabilanDialog() override;

    /*!
     * \brief Этапы работы мастера The Stages enum
     */
    enum Stages
    {
          stgInvite = 0      ///< Приглашение включить устройство
        , stgCalibrateUp     ///< Подъем устройства и калибровка на весу
        , stgCalibrateDown   ///< Устройство на полу, подкручивание ножки до необходимого разброса сил и калибровка
        , stgFinal           ///< Контроль нулевых значений после окончания калибровки
    };


protected:
    void timerEvent(QTimerEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_settingsClicked();
    void on_nextClicked();
    void on_backClicked();
    void on_timeBeforeCalibrateChanged(int value);

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);
    void on_error(const int errorCode);

private:
    Ui::InitialSetupStabilanDialog *ui;

    void supportStat(const double a, const double b, const double c, const double d, double &max, double &maxDiff);

    void changeStage(const bool isNext);

    void connectToDevice();

    DeviceProtocols::Ports m_port;
    QJsonObject m_params;
    QString m_comment;
    Stages m_stage {stgInvite};
    int m_tmCalibrDelay {-1};
    int m_timerCount {-1};

    double m_maxDiff {0.2};
    double m_minWeight {0.1};

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::StabControl* m_stabControl {nullptr};             ///< Управление стабилографией в драйвере
    QMediaPlayer m_player;
};

#endif // INITIALSETUPSTABILANDIALOG_H
