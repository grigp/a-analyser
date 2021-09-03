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

class TetrisGlass;

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

private:
    Ui::TrenTetrisExecute *ui;

    ///< Слои игрового поля
    enum ZLevels
    {
        zlvlGlass = 3,
        zlvlMarker = 4
    };

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
     * \brief Создает и возвращает массив фигур
     */
    QVector<QVector<QColor>> newFigure();

    TrenTetrisDefines::MovingMode m_movingMode {TrenTetrisDefines::mmTake};
    TrenTetrisDefines::ComplexityMode m_complexityMode {TrenTetrisDefines::cmFigures};
    TrenTetrisDefines::DeletingMode m_deletingMode {TrenTetrisDefines::dmRows};

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

    QJsonObject m_markerObj;            ///< Объект с данными маркера
    GraphicCommon::MarkerElement *m_marker {nullptr};
    qreal m_offsX {0};                 ///< Координаты смещения маркера относительно центра фигуры при захвате
    qreal m_offsY {0};

    QJsonObject m_glassObj;            ///< Объект с данными стакана
    TetrisGlass* m_glass {nullptr};

    QVector<QVector<QColor>> nextFigure; ///< Следующая фигура

    QLabel* m_lblRowsDel {nullptr};
    int m_rowsDeleted {0};

    QWidget* m_nextFigure {nullptr};    ///< Виджет для отображения следующей фигуры
    QWidget* m_nextFigurePW {nullptr};  ///< Виджет для отображения следующей фигуры в окне пациента

    TrenTetrisDefines::TakeModeStage m_tmStage {TrenTetrisDefines::tmsTake};   ///< Этап а режиме захвата - укладки
};

#endif // TRENTETRISEXECUTE_H
