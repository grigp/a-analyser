#ifndef TRENEXECUTE_H
#define TRENEXECUTE_H

#include <QFrame>
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
     * Все управление сценой, маркеры и т.д. писать в перекрытом методе в наследниках
     */
    virtual void elementsInteraction();

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

private slots:
    void getData(DeviceProtocols::DeviceData *data);

private:
    QGraphicsScene* m_scene {nullptr};
    double m_prop = 1;   ///< Пропорция для пересчета базовой сцены 2000 x 2000 в реальные размеры игровой сцены
    double m_propX = 1;
    double m_propY = 1;

    int m_frequency = 50;        ///< Частота дискретизации
    DataDefines::PatientKard m_kard;

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::DecartCoordControl* m_dcControl;  ///< Управление декартовыми данными в драйвере
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

private:
    Ui::TrenExecute *ui;
};

#endif // TRENEXECUTE_H
