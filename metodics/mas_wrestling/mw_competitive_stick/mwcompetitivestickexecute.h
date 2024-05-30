#ifndef MWCOMPETITIVESTICKEXECUTE_H
#define MWCOMPETITIVESTICKEXECUTE_H

#include <QWidget>
#include <QMediaPlayer>

#include "datadefines.h"
#include "deviceprotocols.h"
#include "metodicdefines.h"

namespace Ui {
class MWCompetitiveStickExecute;
}

class TestResultData;
class DynamoSignal;

/*!
 * \brief Виджет выполнения теста с соревновательной палкой The MWCompetitiveStickExecute class
 */
class MWCompetitiveStickExecute : public QWidget
{
    Q_OBJECT

public:
    explicit MWCompetitiveStickExecute(QWidget *parent = nullptr);
    ~MWCompetitiveStickExecute() override;

    void setParams(const QJsonObject &probeParams);

protected:
    void closeEvent(QCloseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void start();

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);
    void on_error(const int errorCode);

    void on_calibrate();
    void on_resetMax();
    void on_scaleChange(int scaleIdx);

    void on_record();

private:
    Ui::MWCompetitiveStickExecute *ui;

    /*!
     * \brief Действия по завершению теста
     */
    void finishTest();

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::TensoControl* m_control {nullptr};
    int m_freq {50};
    double m_minVal {0};  ///< Минимальное значение усилия в драйвере
    double m_maxVal {0};  ///< Максимальное значение усилия в драйвере

    DataDefines::PatientKard m_kard;
    TestResultData *m_trd {nullptr};  ///< Объект, записывающий данные в базу

    bool m_isAutoEnd {true};
    int m_recLength {20};

    bool m_isRecording {false};
    int m_recCount {0};

    double m_value {0};     ///< Текущее значение усилия
    double m_valueMax {0};  ///< Максимальное значение усилия, развитое

    DynamoSignal* m_dynamo {nullptr};

    QMediaPlayer m_player;
};

#endif // MWCOMPETITIVESTICKEXECUTE_H
