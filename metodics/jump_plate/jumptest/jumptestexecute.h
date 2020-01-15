#ifndef JUMPTESTEXECUTE_H
#define JUMPTESTEXECUTE_H

#include <QWidget>

#include "datadefines.h"
#include "deviceprotocols.h"
#include "jumptestdefines.h"

namespace Ui {
class JumpTestExecute;
}

class JumpHeightSingleData;
class HoppingData;

/*!
 * \brief Виджет выполнения тестирования измерения высоты прыжка JumpTestExecute class
 */
class JumpTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit JumpTestExecute(QWidget *parent = 0);
    ~JumpTestExecute();

    void setParams(const QJsonObject &params);

protected:
    void timerEvent(QTimerEvent* event);

private slots:
    void start();

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    void showTools(bool isShow);
    void saveResult();
    void cancelTest();

private:
    Ui::JumpTestExecute *ui;

    void methodicWorking(const bool isStart);
    void methodicWorkingJumpHeight(const bool isStart, const int platformsCount);
    void methodicWorkingHopping(const bool isStart, const int platformsCount);

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::JumpPlateControl* m_jumpControl;  ///< Управление прыжковой платформой в драйвере

    JumpTestDefines::Methodic m_methodic = JumpTestDefines::MetJumpHeight;      ///< Код методики

    bool m_plt1Pressed {false};  ///< Нажата ли платформа
    bool m_plt2Pressed {false};
    double m_plt1Time {0};       ///< Время
    double m_plt2Time {0};
    double m_plt1Height {0};     ///< Высота рассчитанная
    double m_plt2Height {0};
    double m_plt1TimeRes {0};    ///< Время результирующее
    double m_plt2TimeRes {0};
    double m_plt1HeightRes {0};  ///< Высота рассчитанная результирующая
    double m_plt2HeightRes {0};
    double m_plt1HoppingTimeOn {0}; ///< Длительность находждения на платформе в методике соскакивания
    double m_plt2HoppingTimeOn {0};

    int m_plt1Count {0};  ///< Кол-во смен состояний
    int m_plt2Count {0};

    bool m_isBlocked {true};     ///< Блокировка обработки событий
    int m_hoppingStage {-1};     ///< Номер этапа в методике соскакивания
    int m_timerScenFailure {-1}; ///< id таймера срыва сценария
    int m_scenFailureStage {-1}; ///< Этап, на котором был запущен таймер

    JumpHeightSingleData *m_jhd;
    HoppingData *m_hd;
};

#endif // JUMPTESTEXECUTE_H
