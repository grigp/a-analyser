#ifndef JUMPTESTEXECUTE_H
#define JUMPTESTEXECUTE_H

#include <QWidget>

#include "datadefines.h"
#include "deviceprotocols.h"

namespace Ui {
class JumpTestExecute;
}

class TestResultData;

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

private slots:
    void start();

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

private:
    Ui::JumpTestExecute *ui;

    void mainMsgData(const bool isStart);

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::JumpPlateControl* m_jumpControl;  ///< Управление прыжковой платформой в драйвере

    TestResultData *m_trd;  ///< Объект, записывающий данные в базу

    bool m_plt1Pressed {false};
    bool m_plt2Pressed {false};
    double m_plt1Time {0};
    double m_plt2Time {0};
    double m_plt1Height {0};
    double m_plt2Height {0};

    int m_plt1Count {0};
    int m_plt2Count {0};
};

#endif // JUMPTESTEXECUTE_H
