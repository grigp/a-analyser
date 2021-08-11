#ifndef TRENTETRISEXECUTE_H
#define TRENTETRISEXECUTE_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>
#include <QMediaPlayer>

#include "datadefines.h"
#include "deviceprotocols.h"
#include "trentetrisdefines.h"
#include "graphiccommon.h"

class Driver;
class TestResultData;
class TrenTetrisPatientWindow;

namespace Ui {
class TrenTetrisExecute;
}

/*!
 * \brief Класс виджета проведения сеанса тренинга Тетрис TrenTetrisExecute class
 */
class TrenTetrisExecute : public QWidget
{
    Q_OBJECT

public:
    explicit TrenTetrisExecute(QWidget *parent = nullptr);
    ~TrenTetrisExecute() override;

    void setParams(const QJsonObject &params);

protected:
    void closeEvent(QCloseEvent *event) override;

    void resizeEvent(QResizeEvent* event) override;

private slots:
    void start();

    void getData(DeviceProtocols::DeviceData *data);
    void on_communicationError(const QString &drvName, const QString &port, const int errorCode);

    void on_selectChannel(int chanIdx);
    void on_zeroing();
    void on_scaleChange(int scaleIdx);
    void on_recording();

    void on_advChannelsClicked(bool checked);


private:
    Ui::TrenTetrisExecute *ui;

    ///< Слои игрового поля
    enum ZLevels
    {
        zlvlBackground = 1,
        zlvlGlass = 2,
        zlvlFigure = 3,
        zlvlMarker = 4
    };

    void setSceneSize(QSize &size);

    void generateNewScene();

    /*!
     * \brief Формирует список каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setChannels();

    void elementsInteraction();

    void showPatientWindow();
    void hidePatientWindow();

    void setMarker(const QJsonObject &objMarker);
    void setBackground(const QJsonObject &objBackground);

    void finishTest();

    void doneDriver();

    QGraphicsScene* m_scene {nullptr};
    double m_prop = 1;   ///< Пропорция для пересчера базовой сцены 2000 x 2000 в реальные размеры игровой сцены
    double m_propX = 1;
    double m_propY = 1;

    int m_frequency = 50;        ///< Частота дискретизации
    DataDefines::PatientKard m_kard;

    Driver* m_driver {nullptr};     ///< Драйвер передающий данные
    DeviceProtocols::DecartCoordControl* m_dcControl;  ///< Управление декартовыми данными в драйвере
    TestResultData *m_trd;  ///< Объект, записывающий данные в базу

    TrenTetrisPatientWindow* m_patientWindow {nullptr};

    bool m_isRecording {false};     ///< Протекает ли запись данных
    int m_recCount {0};             ///< Счетчик пакетов данных в пробе
    int m_recLength {0};            ///< Длительность записи

    TrenTetrisDefines::MovingMode m_movingMode {TrenTetrisDefines::mmTake};
    TrenTetrisDefines::ComplexityMode m_complexityMode {TrenTetrisDefines::cmFigures};
    TrenTetrisDefines::DeletingMode m_deletingMode {TrenTetrisDefines::dmRows};

    int m_glassHCount {21};         ///< Кол-во позиций в стакане по горизонтали
    int m_glassVCount {15};         ///< Кол-во позиций в стакане по вертикали

    QString m_cubeImageFileName {""}; ///< Имя файла, содержащего картинку кубика

    ///< Цвета в режиме удаления строк (обычный, не цветной, вариант)
    QColor m_cubeColor {Qt::yellow};    ///< Цвет перемещаемой фигуры
    QColor m_glassColor {Qt::cyan};     ///< Цвет фигур, уложенных в стакан
    QColor m_lastColor {Qt::magenta};   ///< Цвет последней уложенной фигуры

    int m_deletingCubeCount {3};      ///< Кол-во кубиков одного цвета, удаляемых одновременно
    QList<QColor> m_colorModeColors;  ///< Цвета для вариантов цветного тетриса

    QJsonObject m_markerObj;            ///< Объект с данными маркера
    QJsonObject m_backgroundObj;        ///< Объект с данными фона
    int m_autoMovingSpeed {3};          ///< Скорость спуска в режиме автоматического спуска

    GraphicCommon::MarkerElement *m_marker {nullptr};
    GraphicCommon::BackgroundElement *m_background {nullptr};

    ///< Границы зоны рамки
    int m_bndLeft {0};
    int m_bndTop {0};
    int m_bndRight {0};
    int m_bndBottom {0};
};

#endif // TRENTETRISEXECUTE_H
