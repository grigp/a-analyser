#ifndef TRENEXECUTE_H
#define TRENEXECUTE_H

#include <QFrame>
#include <QLabel>
#include <QWidget>
#include <QJsonObject>
#include <QGraphicsScene>

#include "datadefines.h"
#include "deviceprotocols.h"
#include "graphiccommon.h"

namespace Ui {
class TrenExecute;
}

class TestResultData;
class TrenagerPatientWindow;
class SettingsValue;

/*!
 * \brief Базовый класс тренажеров с получением данных от устройства TrenExecute class
 */
class TrenExecute : public QWidget
{
    Q_OBJECT

public:
    explicit TrenExecute(QWidget *parent = nullptr);
    ~TrenExecute() override;

    virtual void setParams(const QJsonObject &params);

protected:
    void closeEvent(QCloseEvent *event) override;

    void resizeEvent(QResizeEvent* event) override;

protected slots:
    virtual void start();

    virtual void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    virtual void on_selectChannel(int chanIdx);
    virtual void on_recording();

    virtual void on_advChannelsClicked(bool checked);

    virtual void on_selectAdvChannelClicked(int index);

    virtual void on_enabledAdvChannelChanged(bool enabled);

protected:
    ///< Слои игрового поля
    enum ZLevels
    {
          zlvlBackground = 1        ///< Слой фона. Всегда внизу
        , zlvlVidioIrritant = 100   ///< Слой видеораздражителя. Оптокинетическая стимуляция
    };

    virtual void setSceneSize(QSize &size);

    virtual void generateNewScene();

    /*!
     * \brief Взаимодействие элементов
     * Все управление сценой, маркеры и т.д. писать в перекрытом методе в подклассах
     */
    virtual void elementsInteraction(DeviceProtocols::DeviceData *data);

    /*!
     * \brief Формирует список каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setMainChannels();

    /*!
     * \brief Формирует список дополнительных каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setAdvancedChannels();

    /*!
     * \brief Устанавливает, будет ли использоваться дополнительный канал управления
     * \param chan - номер канала
     * \param enable - будет ли использоваться
     */
    void setAdvancedChannelEnable(const int chan, const bool enable);

    /*!
     * \brief Устанавливает название действия, связанного с дополнительным каналом управления
     * \param chan - номер канала
     * \param title - название функции
     */
    void setAdvancedFunctionTitle(const int chan, const QString title);

    virtual void showPatientWindow();
    virtual void hidePatientWindow();

    virtual void setBackground(const QJsonObject &objBackground);

    /*!
     * \brief Устанавливает объект - раздражитель на сцену
     */
    virtual void setVideoIrritant();

    virtual void finishTest();

    virtual void doneDriver();

    /*!
     * \brief Здесь подклассы должны заполнить frame своими управляющими элементами игры
     * \param frame - указатель на фрейм, куда добавлять
     */
    virtual void fillGameControl(QFrame *frame);

    /*!
     * \brief Здесь подклассы должны заполнить frame своими управляющими элементами подсказок для игры
     * \param frame - указатель на фрейм, куда добавлять
     */
    virtual void fillGameHints(QFrame *frame);

    /*!
     * \brief Здесь подклассы должны заполнить frame своими элементами, отображающими процесс игры
     * \param frame - указатель на фрейм, куда добавлять
     */
    virtual void fillGameParams(QFrame *frame);

    /*!
     * \brief Возвращает uid выбранного канала
     */
    QString currentChannelUID();

    /*!
     * \brief Возвращает указатель на драйвер
     */
    Driver* driver() {return  m_driver;}
    QGraphicsScene* scene() {return  m_scene;}

    double prop() {return m_prop;}
    double propX() {return m_propX;}
    double propY() {return m_propY;}
    int bndLeft() {return m_bndLeft;}
    int bndTop() {return m_bndTop;}
    int bndRight() {return m_bndRight;}
    int bndBottom() {return m_bndBottom;}

    /*!
     * \brief Возвращает частоту дискретизации
     */
    int frequency() {return  m_frequency;}

    /*!
     * \brief Возвращает ширину фрейма управления
     */
    int getFrameControlWidth();

    /*!
     * \brief Возвращает ширину фрейма параметров в окне пациента
     */
    int pwGetFrameParamsWidth();

    /*!
     * \brief Изменяет значение числа очков
     * Значение кол-ва очков не может быть отрицательным
     * \param value - значение, на которое меняем. Если +, то увеличиваем, если -, уменьшаем
     */
    void changeGameScore(const int value);

    /*!
     * \brief Добавляет метку для отображения игрового параметра на окне пациента
     * \param text - имя метки
     * \param styleSheet - визуальный стиль
     */
    void pwSetGameParamLabel(const QString text, const QString styleSheet);
    /*!
     * \brief Изменяет значение текста метки игрового параметра по индексу параметра на окне пациента
     * \param idxParam - индекс параметра
     * \param value - значение
     */
    void pwSetGameParamLabelValue(const int idxParam, const QString value);

    /*!
     * \brief Добваляет виджет для отображения подсказок
     * \param widget - указатель на добавляемый виджет
     */
    void pwAddHintWidget(QWidget* widget);

    /*!
     * \brief Возвращает указатель на элемент игровой сцены - фон
     */
    GraphicCommon::BackgroundElement* background() {return m_background;}

    /*!
     * \brief Возвращает указатель на элемент игровой сцены - видео раздражитель
     */
    GraphicCommon::VidioIrritant* videoIrritant() {return m_videoIrritant;}

    /*!
     * \brief Возвращает указатель на окно пациента
     */
    TrenagerPatientWindow* patientWindow() {return m_patientWindow;}

    bool isClosed() const {return m_isClosed;}

    /*!
     * \brief Структура для записи показателей тренажеров GameFactors struct
     */
    struct GameFactors
    {
        QString uid;
        double value;
    };
    void addFactorValue(const QString &uid, const double value);

    /*!
     * \brief Возвращает кол-во отсчетов после начала записи
     */
    int recCount() const {return m_recCount;}

    /*!
     * \brief Возвращает true, если проходит запись
     */
    bool isRecording() const {return m_isRecording;}

    bool isPhisioChannel() const {return m_isPhisioChannel;}
    int boundForce() const {return m_boundForce;}
    int boundMyogram() const {return m_boundMyogram;}
    double advancedValue(const int chan) const;
    /*!
     * \brief Возвращает true, если физиологический канал разрешен и превысил пороговое значение
     * Возвращает true, если физиологический канал запрещен
     */
    bool isAdvancedChannelAboveBound(const int chan);
    /*!
     * \brief Возвращает true, если физиологический канал разрешен и только что превысил пороговое значение
     * Возвращает false, если физиологический канал запрещен
     */
    bool isAdvancedChannelAboveBoundNow(const int chan);

    virtual QString getAutoSaveParamsSectionName() const {return "Trenager";}

private slots:
    void getData(DeviceProtocols::DeviceData *data);

private:
    Ui::TrenExecute *ui;

    QGraphicsScene* m_scene {nullptr};
    double m_prop = 1;   ///< Пропорция для пересчета базовой сцены 2000 x 2000 в реальные размеры игровой сцены
    double m_propX = 1;
    double m_propY = 1;

    int m_frequency = 50;        ///< Частота дискретизации
    DataDefines::PatientKard m_kard;

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    TestResultData *m_trd;  ///< Объект, записывающий данные в базу

    TrenagerPatientWindow* m_patientWindow {nullptr};

    bool m_isRecording {false};     ///< Протекает ли запись данных
    int m_recCount {0};             ///< Счетчик пакетов данных в пробе
    int m_recLength {0};            ///< Длительность записи

    QJsonObject m_backgroundObj;        ///< Объект с данными фона

    GraphicCommon::BackgroundElement *m_background {nullptr};

    GraphicCommon::VidioIrritant* m_videoIrritant {nullptr};   ///< Слой видеораздражителя (оптокинетической стимуляции)

    ///< Границы зоны рамки
    int m_bndLeft {0};
    int m_bndTop {0};
    int m_bndRight {0};
    int m_bndBottom {0};

    QLabel* m_lblGameScore {nullptr};    ///< Индикатор очков игры
    int m_gameScore {0};                 ///< Значние кол-ва очков
    QList<GameFactors> m_gameFactors;    ///< Список показателей тренажера. Заполняется подклассами через addFactorValue

    bool m_isClosed {false};

    bool m_isPhisioChannel {false};     ///< Будет ли использоваться дополнительный канал физиологии
    int m_boundForce {20};              ///< Пороговое значение для силомера
    int m_boundMyogram {200};           ///< Пороговое значение для миограммы
    double m_adv0Value {0};           ///< Значение канала физиологии 0
    double m_adv1Value {0};           ///< Значение канала физиологии 1
    bool m_isAdv0ChannelAboveBound {false};  ///< превышает ли значение физиологического канала 0 порог
    bool m_isAdv1ChannelAboveBound {false};  ///< превышает ли значение физиологического канала 1 порог

    SettingsValue* m_AdvChannel0Select  {nullptr};  ///< Выбранный дополнительный канал 0 - параметр из хранилища
    SettingsValue* m_AdvChannel1Select  {nullptr};  ///< Выбранный канал 1 - параметр из хранилища
    SettingsValue* m_AdvChannel0Enabled {nullptr};  ///< Будет ли управление по дополнительному каналу 0 - параметр из хранилища
    SettingsValue* m_AdvChannel1Enabled {nullptr};  ///< Будет ли управление по дополнительному каналу 1 - параметр из хранилища
};

#endif // TRENEXECUTE_H
