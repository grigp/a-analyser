#ifndef DROPTESTEXECUTE_H
#define DROPTESTEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QStandardItemModel>

#include "jumpplatedefines.h"
#include "deviceprotocols.h"
#include "datadefines.h"

namespace Ui {
class DropTestExecute;
}

class Driver;

/*!
 * \brief Виджет выполнения дроп теста DropTestExecute class
 */
class DropTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit DropTestExecute(QWidget *parent = 0);
    ~DropTestExecute();

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
    Ui::DropTestExecute *ui;

    /*!
     * \brief Этапы дроп прыжка Stages enum
     */
    enum Stage
    {
          dtsWaiting = 0   ///< Ожидание прыжка. Платформа пуста
        , dtsContact       ///< Контактная фаза. Пребывание на платформе
        , dtsJump          ///< Бесконтактная фаза. Прыжок. Платформа пуста
        , dtsResult        ///< Окончание дроп прыжка. Вывод результата. Заканчивается уходом с платформы
    };

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

    void setStage(const Stage stage);

    void setModelGeometry();

    void finishTest();

    DataDefines::PatientKard m_kard;

    JumpPlateDefines::TestFinishKind m_testFinishKind {JumpPlateDefines::tfkQuantity};
    int m_jumpsMax {1};
    int m_testTime {30};

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::JumpPlateControl* m_jumpControl;  ///< Управление прыжковой платформой в драйвере

    bool m_isBlocked {true};     ///< Блокировка обработки событий
    bool m_plt1Pressed {false};  ///< Нажата ли платформа
    bool m_plt2Pressed {false};
    double m_plt1Time {0};       ///< Время платформы
    double m_plt2Time {0};
    double m_timeContact {0};    ///< Время контактной фазы
    double m_timeNoContact {0};  ///< Время бесконтактной фазы

    int m_jumpsCount {0};        ///< Счетчик прыжков
    int m_timeCount {0};         ///< Счетчик времени теста
    int m_timerEndOfTest {-1};   ///< id таймера времени теста
    bool m_isRecording {false};  ///< Проходит ли запись

    Stage m_stage {dtsWaiting};  ///< Текущий этап прыжка

    QStandardItemModel m_mdlTable;

    ///< Ускорение свободного падения. Зависит от местоположения, которое необходимо выбрать в настройках программы
    double m_g {9.8};
};

#endif // DROPTESTEXECUTE_H
