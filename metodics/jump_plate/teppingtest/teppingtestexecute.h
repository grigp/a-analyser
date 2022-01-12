#ifndef TEPPINGTESTEXECUTE_H
#define TEPPINGTESTEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QStandardItemModel>

#include "jumpplatedefines.h"
#include "deviceprotocols.h"
#include "basedefines.h"

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

    void on_selectGraph();
    void on_selectBar();
    void on_select3D(bool checked);

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
    void iterate(const bool isStart, BaseDefines::Side side = BaseDefines::NoSide);

    void setModelGeometry();

    void finishTest();

    void setDiagParams();

    void restoreGraphParams();

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
    double m_plt1TimeNoContact {0}; ///< Время бесконтактной фазы
    double m_plt2TimeNoContact {0};

    int m_stepsLeftCount {0};
    int m_stepsRightCount {0};
    int m_timeCount {0};
    int m_time {0};
    int m_timerEndOfTest {-1};   ///< id таймера времени теста
    int m_timerBeforeRun {-1};   ///< id таймера запуска записи
    int m_runCounter {3};        ///< Счетчик времени запуска записи

    bool m_isRecording {false};  ///< Проходит ли запись
    QStandardItemModel m_mdlLeft;
    QStandardItemModel m_mdlRight;
};

#endif // TEPPINGTESTEXECUTE_H
