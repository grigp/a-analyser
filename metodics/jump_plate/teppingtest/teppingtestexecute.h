#ifndef TEPPINGTESTEXECUTE_H
#define TEPPINGTESTEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QStandardItemModel>

#include "jumpplatedefines.h"
#include "deviceprotocols.h"
#include "baseutils.h"

namespace Ui {
class TeppingTestExecute;
}

class Driver;

/*!
 * \brief Виджет выполнения теппинг - теста TeppingTestExecute class
 */
class TeppingTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit TeppingTestExecute(QWidget *parent = 0);
    ~TeppingTestExecute();

    void setParams(const QJsonObject &params);

protected:
    void closeEvent(QCloseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private slots:
    void start();

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    void on_recording();

private:
    Ui::TeppingTestExecute *ui;

    ///< Роли в таблице результатов теста
    enum JumpesTableRoles
    {
        NumberRole = Qt::UserRole + 1   ///< Доступ к номеру шага в столбце 0. int
      , ValueRole                       ///< Доступ к данных в столбцах 1 и 2 - времени. double
    };

    /*!
     * \brief Итерация выполнения методики
     * \param isStart - признак запуска на старте
     */
    void iterate(const bool isStart, BaseUtils::Side side = BaseUtils::NoSide);

    void setModelGeometry();

    void finishTest();

    JumpPlateDefines::TestFinishKind m_testFinishKind;
    int m_stepsMax {20};
    int m_testTime {10};

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::JumpPlateControl* m_jumpControl;  ///< Управление прыжковой платформой в драйвере

    bool m_isBlocked {true};     ///< Блокировка обработки событий
    bool m_plt1Pressed {false};  ///< Нажата ли платформа
    bool m_plt2Pressed {false};
    double m_plt1Time {0};       ///< Время платформы
    double m_plt2Time {0};

    int m_stepsLeftCount {0};
    int m_stepsRightCount {0};
    int m_timeCount {0};
    int m_time {0};
    int m_timerEndOfTest {-1};   ///< id таймера времени теста

    bool m_isRecording {false};  ///< Проходит ли запись
    QStandardItemModel m_mdlLeft;
    QStandardItemModel m_mdlRight;
};

#endif // TEPPINGTESTEXECUTE_H
