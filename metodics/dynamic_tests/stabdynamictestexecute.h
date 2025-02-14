#ifndef STABDYNAMICTESTEXECUTE_H
#define STABDYNAMICTESTEXECUTE_H

#include <QWidget>
#include <QFrame>
#include <QJsonObject>
#include <QMediaPlayer>

#include "datadefines.h"
#include "crossdefines.h"
#include "deviceprotocols.h"
#include "metodicdefines.h"

class BilateralResultData;

namespace Ui {
class StabDynamicTestExecute;
}

class TestResultData;
class Stabilogram;
class Balistogram;
class Driver;
class StabDynamicTestPatientWindow;

/*!
 * \brief Базовый класс виджета выполнения стабилометрического динамического теста StabDynamicTestExecute class
 */
class StabDynamicTestExecute : public QWidget
{
    Q_OBJECT

public:
    explicit StabDynamicTestExecute(QWidget *parent = nullptr);
    ~StabDynamicTestExecute() override;

    virtual void setParams(const QJsonObject &params);

protected:
    void closeEvent(QCloseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    double x() {return m_x;}
    double y() {return m_y;}

    /*!
     * \brief Устанавливает заголовок окна
     */
    void setTitle(const QString &title);

    TestResultData *trd() {return m_trd;}

    DeviceProtocols::StabControl* stabControl() {return m_stabControl;}

    Driver* driver() {return m_driver;}

    bool isRecording() const {return m_isRecording;}
    bool isPause() const {return m_isPause;}
    void setIsPause(const bool isp) {m_isPause = isp;} ///< Пауза может быть по любой причине и управлять ею надо в подклассах

    int recCount() const {return m_recCount;}
    int freqStab() const {return m_freqStab;}
    int freqZ() const {return m_freqZ;}
    int diap() const;

    void isShowValues(const bool isShow);
    void isTraceControl(const bool isTrace);

    /*!
     * \brief Устанавливает текст метки "Длительность записи"
     */
    void setRecordLengthTitle(const QString title);

    void setVisibleRecordLength(const bool visible);

    /*!
     * \brief Устанавливает длительность записи для отображения
     * \param length - Длительность записи в пакетах
     */
    void setRecordLength(const int length);

    /*!
     * \brief Принудительная установка индикатора времени записи
     * \param pos - позиция в пакетах
     * \param recLen - длительность в пакетах
     */
    void setRecordPosition(const int pos, const int recLen);

    /*!
     * \brief Возвращает id выбранного канала
     */
    QString selectedChannel() const;

    /*!
     * \brief Виртуальная функция, показывающая, будет ли запись завершаться автоматически или вручную
     * Подклассы могут менять это поведение, и запись может быть завершена вручную
     * \return true - запись завершается автоматически по истечению сценария
     */
    virtual bool isAutoFinishRecord() {return true;}

    /*!
     * \brief Добавляет маркер
     */
    void addMarker();

    /*!
     * \brief Управление видимостью маркера
     */
    void setVisibleMarker(const bool isVisible);

    /*!
     * \brief Добавляет метку цели
     * \param x, y - координаты цели
     * \param colorBackground - цвет фона
     * \param colorBorder - цвет границы
     * \param visible - видимость
     */
    void addTarget(const double x, const double y, const QColor colorBackground, const QColor colorBorder, bool visible = true);

    /*!
     * \brief setTarget - Устанавливает цель на позицию
     * \param x, y - координаты цели
     * \param idx - номер цели в списке
     */
    void setTarget(const double x, const double y, const int idx = 0);

    /*!
     * \brief Очищает список целей
     */
    void clearTargets();

    /*!
     * \brief Виртуальная функция, создающая окно пациента. Должна быть перекрыта в подклассах
     */
    virtual StabDynamicTestPatientWindow* createPatientWindow();

    /*!
     * \brief Действия по завершению теста
     */
    virtual void finishTest();

    /*!
     * \brief Виртуальная функция заполнения панели специфичных элеменов управления для методики
     * \param frSpecific - указатель на фрейм
     */
    virtual void fillSpecific(QFrame *frSpecific);

    /*!
     * \brief Возвращает список этапов для режима автоматичекого проведения
     * Перекрывать, если отличаются от MetodicDefines::AutoStagesBase
     */
    virtual QList<MetodicDefines::AutoModeStaticStages> getAutoModeStaticStages();

protected slots:
    virtual void start();

    virtual void getData(DeviceProtocols::DeviceData *data);
    virtual void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    /*!
     * \brief Управляет началом / остановом / прерыванием записи
     */
    virtual void recording();

    /*!
     * \brief Устанавливает диапазон в окне пациента
     * \param diap - значение диапазона
     */
    virtual void setPatientWinDiap(const int diap);

    /*!
     * \brief Функция, помещающая комментарий поверх окна
     * \param comment - текст комментария
     */
    virtual void setFrontComment(const QString& comment, const bool isPatientOnly = false, const QString& styleSheet = "");

private slots:
    void on_started();

    void scaleChange(int scaleId);


    /*!
     * \brief Центровка
     */
    void zeroing();

    /*!
     * \brief Калибровка
     */
    void calibrate();

    void on_advChannelsClicked(bool checked);

    void splitterMoved(int pos,int index);

private:
    Ui::StabDynamicTestExecute *ui;

    /*!
     * \brief Формирует список каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setChannels();

    /*!
     * \brief Создает и показывает окно пациента
     */
    void createAndShowPatientWindow();

    /*!
     * \brief Прячет окно пациента
     */
    void hidePatientWindow();

    /*!
     * \brief Возвращает строку с текстом ожидания события
     * \param eventName - название события
     * \param sec - время до центро
     */
    QString msgWaitEvent(const QString& eventName, const int sec) const;

    ///< Координаты маркера
    double m_x {0};
    double m_y {0};
    double m_z {0};

    int m_freqStab {50};        ///< Частота дискретизации стабилограммы
    int m_freqZ {50};           ///< Частота дискретизации баллистограммы

    DataDefines::PatientKard m_kard;
    MetodicDefines::MetodicInfo m_metInfo;

    bool m_isRecording {false};     ///< Протекает ли запись данных
    bool m_isPause {false};         ///< Есть ли пауза в записи в настоящий момент
                                    ///< Пауза может быть по любой причине и управлять ею надо в подклассах
    int m_recCount {0};             ///< Счетчик пакетов данных в пробе
    int m_recLength {0};            ///< Длительность записи

    Driver* m_driver {nullptr};                             ///< Драйвер передающий данные
    DeviceProtocols::StabControl* m_stabControl {nullptr};  ///< Управление стабилографией в драйвере

    TestResultData *m_trd;  ///< Объект, записывающий данные в базу

    StabDynamicTestPatientWindow *m_patientWin {nullptr};

    DeviceProtocols::MultiPlatformControl* m_bilatControl {nullptr};   ///< Управление билатаральным режимом в драйвере
    QRect m_platform1 {QRect(0, 0, 0, 0)};   ///< Платформы в билатеральном режиме
    QRect m_platform2 {QRect(0, 0, 0, 0)};
    BilateralResultData* m_rdBilat {nullptr};

    int m_autoModeTimerId {-1};   ///< id таймера для режима автономной работы
    int m_stageNum {0};           ///< Номер этапа в автоматическом режиме
    int m_autoModeSecCounter {0};  ///< Счетчик секунд в автоматическом режиме
    int m_autoTimeRun {5};         ///< Время задержки до операции
    int m_autoTimeLatent {2};      ///< Длительность латентного периода
    QList<MetodicDefines::AutoModeStaticStages> m_stages;  ///< Список этапов для автоматического режима

    QMediaPlayer m_player;
};

#endif // STABDYNAMICTESTEXECUTE_H
