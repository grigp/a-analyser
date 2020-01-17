#ifndef JUMPHEIGHTTESTEXECUTE_H
#define JUMPHEIGHTTESTEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QStandardItemModel>

#include "jumpplatedefines.h"
#include "jumpheighttestdefines.h"
#include "deviceprotocols.h"


namespace Ui {
class JumpHeightTestExecute;
}

class Driver;

/*!
 * \brief Виджет выполнения тестирования измерения высоты серии прыжков JumpHeightTestExecute class
 */
class JumpHeightTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit JumpHeightTestExecute(QWidget *parent = 0);
    ~JumpHeightTestExecute();

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
    Ui::JumpHeightTestExecute *ui;

    ///< Роли в таблице результатов теста
    enum JumpesTableRoles
    {
          NumberRole = Qt::UserRole + 1   ///< Доступ к номеру прыжка в столбце 0. int
        , ValueRole                       ///< Доступ к данных в столбцах 1 и 2 - высоте прыжка и времени. double
    };

    /*!
     * \brief Итерация выполнения методики
     * \param isStart - признак запуска на старте
     */
    void iterate(const bool isStart);

    void setModelGeometry();

    void finishTest();

    JumpPlateDefines::TestFinishKind m_testFinishKind {JumpPlateDefines::tfkQuantity};
    JumpHeightTestDefines::Strategy m_strategy {JumpHeightTestDefines::jhsMaxHeight};
    int m_jumpsMax {1};
    int m_testTime {30};
    int m_contactTimeBound {200};

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::JumpPlateControl* m_jumpControl;  ///< Управление прыжковой платформой в драйвере

    bool m_isBlocked {true};     ///< Блокировка обработки событий
    bool m_plt1Pressed {false};  ///< Нажата ли платформа
    bool m_plt2Pressed {false};
    double m_plt1Time {0};       ///< Время платформы
    double m_plt2Time {0};
    double m_time {0};
    double m_plt1Height {0};     ///< Высота прыжка на платформе
    double m_plt2Height {0};
    double m_height {0};
    int m_jumpsCount {0};        ///< Счетчик прыжков
    int m_timeCount {0};         ///< Счетчик времени теста
    int m_timerEndOfTest {-1};   ///< id таймера времени теста
    bool m_isRecording {false};  ///< Проходит ли запись

    QStandardItemModel m_mdlTable;
};


#endif // JUMPHEIGHTTESTEXECUTE_H
