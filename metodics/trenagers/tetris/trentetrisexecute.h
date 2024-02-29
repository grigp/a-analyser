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
#include "trenstabexecute.h"

/*!
 * \brief Структура параметров звуковой схемы SoundSheme struct
 */
struct SoundSheme
{
    QString take;
    QString put;
    QString deleteRow;
};


class TetrisGlass;
class TakePutResultData;

namespace Ui {
class TrenTetrisExecute;
}

/*!
 * \brief Класс виджета проведения сеанса тренинга Тетрис TrenTetrisExecute class
 */
class TrenTetrisExecute : public TrenStabExecute
{
    Q_OBJECT

public:
    explicit TrenTetrisExecute(QWidget *parent = nullptr);
    ~TrenTetrisExecute() override;

    void setParams(const QJsonObject &params) override;

protected slots:
    void on_recording() override;

protected:
    void timerEvent(QTimerEvent *event) override;

    /*!
     * \brief информирует от смене этапа для автоматического режима
     * \param stage - новый этап
     */
    void autoModeStageChanged(const MetodicDefines::AutoModeStaticStages stage) override;

    /*!
     * \brief Формирует список дополнительных каналов для выбора управления
     * По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
     */
    void setAdvancedChannels() override;

    /*!
     * \brief Взаимодействие элементов
     * Все управление сценой, маркеры и т.д.
     */
    void elementsInteraction(DeviceProtocols::DeviceData *data) override;

    /*!
     * \brief Ограничение хождения маркера в зоне стакана
     * \param mx, my - координаты маркера вход и выход
     */
    void boundingMarker(double &mx, double &my);

    /*!
     * \brief Ограничение хождения фигуры в зоне стакана
     * \param mx, my - координаты фигуры вход и выход
     */
    void boundingFigure(double &mx, double &my);

    /*!
     * \brief Взаимодействие элементов в режиме захвата - укладки
     * \param mx, my - координаты маркера
     */
    void takeModeInteraction(double &mx, double &my);

    void autoDescentModeInteraction(double &mx, double &my);

    /*!
     * \brief Укладывает фигуру
     */
    void putFigure();

    void generateNewScene() override;

    /*!
     * \brief Заполнить frame своими элементами, отображающими процесс игры
     * \param frame - указатель на фрейм, куда добавлять
     */
    void fillGameParams(QFrame *frame) override;

    /*!
     * \brief Заполнить frame своими элементами, отображающими подсказки в процессе игры
     * \param frame - указатель на фрейм, куда добавлять
     */
    void fillGameHints(QFrame *frame) override;

    void finishTest() override;

    void initFinishTest() override;

    QString getAutoSaveParamsSectionName() const override {return "TrenagerType2";}

private:
    Ui::TrenTetrisExecute *ui;

    ///< Слои игрового поля
    enum ZLevels
    {
        zlvlGlass = 3,
        zlvlMarker = 4
    };

    /*!
     * \brief Применяет параметры теста
     */
    void assignParams();

    /*!
     * \brief Установка маркера на игровую сцену
     * \param objMarker - объект описания маркера из параметров
     */
    void setMarker(const QJsonObject &objMarker);

    /*!
     * \brief Установка стакана на игровую сцену
     * \param objGlass - объект описания стакана из параметров
     */
    void setGlass(const QJsonObject &objGlass);


    /*!
     * \brief Изменяет значение кол-ва удаленных строк
     * \param value - значение, на которое меняем.
     */
    void changeRowsDeleted(const int value);

    /*!
     * \brief Сброс цвета последнего выделенного в режиме построчного удаления
     */
    void clearLastColor();

    /*!
     * \brief Анализ и удаление строк
     */
    void deleteRows();

    /*!
     * \brief Удаление строки. Все строки, начиная с row + 1 смещаются вниз
     * \param row - номер строки
     */
    void deleteRow(const int row);

    /*!
     * \brief Анализ и удаление кубиков одного цвета
     * \param lastFigCubes - список кубиков последней фигуры
     */
    void deleteOneColorCubes(const QList<QPoint> lastFigCubes);

    /*!
     * \brief Рекурсивная функция заполнения списка позиций с кубиками одного цвета
     * \param oneColorCubes - заполняемый список
     * \param pos - позиция
     * \param color - цвет
     */
    void fillOneColorCubesList(QList<QPoint>& oneColorCubes, const QPoint pos, const QColor color) const;

    /*!
     * \brief Сдвигает кубики в столбце стакана вниз
     * \param x - номер столбца
     * \param y - начальная позиция сдвига
     */
    void shiftCol(const int x, const int y);

    /*!
     * \brief Анализ стакана на заполнение
     * \return true, если стакан заполнен (есть элементы на последней строке)
     */
    bool isGlassFull();

    /*!
     * \brief Создает и возвращает массив фигур
     */
    QVector<QVector<QColor>> newFigure();

    /*!
     * \brief Отложенное по таймеру удаление строк
     */
    void doDelRows();

    /*!
     * \brief Отложенное по таймеру удаление болков одного цвета
     */
    void doDelOneColor();

    TrenTetrisDefines::MovingMode m_movingMode {TrenTetrisDefines::mmTake};
    TrenTetrisDefines::ComplexityMode m_complexityMode {TrenTetrisDefines::cmFigures};
    TrenTetrisDefines::DeletingMode m_deletingMode {TrenTetrisDefines::dmRows};
    bool m_isShowGrid {false};

    int m_glassHCount {15};         ///< Кол-во позиций в стакане по горизонтали
    int m_glassVCount {21};         ///< Кол-во позиций в стакане по вертикали

    QString m_cubeImageFileName {""}; ///< Имя файла, содержащего картинку кубика

    ///< Цвета в режиме удаления строк (обычный, не цветной, вариант)
    QColor m_cubeColor {Qt::yellow};    ///< Цвет перемещаемой фигуры
    QColor m_glassColor {Qt::cyan};     ///< Цвет фигур, уложенных в стакан
    QColor m_lastColor {Qt::magenta};   ///< Цвет последней уложенной фигуры
    QColor m_deletingColor {Qt::lightGray};  ///< Цвет фигур, подлежащих удалению

    int m_deletingCubeCount {3};      ///< Кол-во кубиков одного цвета, удаляемых одновременно
    QList<QColor> m_colorModeColors;  ///< Цвета для вариантов цветного тетриса

    int m_autoMovingSpeed {3};          ///< Скорость спуска в режиме автоматического спуска
    qreal m_autoY {0};                  ///< Положение фигуры по y в режиме автоматического спуска

    QJsonObject m_markerObj;            ///< Объект с данными маркера
    GraphicCommon::MarkerElement *m_marker {nullptr};
    qreal m_offsX {0};                 ///< Координаты смещения маркера относительно центра фигуры при захвате
    qreal m_offsY {0};

    QJsonObject m_glassObj;            ///< Объект с данными стакана
    TetrisGlass* m_glass {nullptr};

    QVector<QVector<QColor>> m_nextFigure; ///< Следующая фигура

    QLabel* m_lblRowsDel {nullptr};
    int m_rowsDeleted {0};

    QWidget* m_wgtNextFigure {nullptr};    ///< Виджет для отображения следующей фигуры
    QWidget* m_wgtNextFigurePW {nullptr};  ///< Виджет для отображения следующей фигуры в окне пациента

    TrenTetrisDefines::TakeModeStage m_tmStage {TrenTetrisDefines::tmsTake};   ///< Этап а режиме захвата - укладки

    QMediaPlayer m_player;
    SoundSheme m_soundSheme;
    TakePutResultData* m_takePutResData {nullptr};

    QList<int> m_fullRows;
    QList<QPoint> m_oneColorCubes;
    int m_tmDelRows {-1};     ///< id таймера удаления строк
    int m_tmDelOneCol {-1};   ///< id таймера удаления блоков одного цвета

    bool m_isRecordingTimeOut {false};
    int m_tmFinishTest {-1};
};

#endif // TRENTETRISEXECUTE_H
