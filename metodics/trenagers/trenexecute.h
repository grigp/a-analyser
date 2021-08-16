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

protected:
    ///< Слои игрового поля
    enum ZLevels
    {
        zlvlBackground = 1  ///< Слой фона. Всегда внизу
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
    void setChannels();

    virtual void showPatientWindow();
    virtual void hidePatientWindow();

    virtual void setBackground(const QJsonObject &objBackground);

    virtual void finishTest();

    virtual void doneDriver();

    /*!
     * \brief Здесь подклассы должны заполнить frame своими управляющими элементами игры
     * \param frame - указатель на фрейм, куда добавлять
     */
    virtual void fillGameControl(QFrame *frame);

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
     * \brief Возвращает указатель на элемент игровой сцены - фон
     */
    GraphicCommon::BackgroundElement* background() {return  m_background;}

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
//    DeviceProtocols::DecartCoordControl* m_dcControl;  ///< Управление декартовыми данными в драйвере
    TestResultData *m_trd;  ///< Объект, записывающий данные в базу

    TrenagerPatientWindow* m_patientWindow {nullptr};

    bool m_isRecording {false};     ///< Протекает ли запись данных
    int m_recCount {0};             ///< Счетчик пакетов данных в пробе
    int m_recLength {0};            ///< Длительность записи

    QJsonObject m_backgroundObj;        ///< Объект с данными фона

    GraphicCommon::BackgroundElement *m_background {nullptr};

    ///< Границы зоны рамки
    int m_bndLeft {0};
    int m_bndTop {0};
    int m_bndRight {0};
    int m_bndBottom {0};

    QLabel* m_lblGameScore {nullptr};    ///< Индикатор очков игры
    int m_gameScore {0};                 ///< Значние кол-ва очков
    QList<GameFactors> m_gameFactors;    ///< Список показателей тренажера. Заполняется подклассами через addFactorValue

    bool m_isClosed {false};
};

#endif // TRENEXECUTE_H
