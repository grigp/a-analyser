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

class Driver;
class TestResultData;

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

protected:
    /*!
     * \brief Взаимодействие элементов
     * Все управление сценой, маркеры и т.д.
     */
    void elementsInteraction(DeviceProtocols::DeviceData *data) override;

    void generateNewScene() override;

    /*!
     * \brief Заполнить frame своими элементами, отображающими процесс игры
     * \param frame - указатель на фрейм, куда добавлять
     */
    void fillGameParams(QFrame *frame) override;


private:
    Ui::TrenTetrisExecute *ui;

    ///< Слои игрового поля
    enum ZLevels
    {
        zlvlMarker = 4
    };

    void setMarker(const QJsonObject &objMarker);

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

    int m_autoMovingSpeed {3};          ///< Скорость спуска в режиме автоматического спуска

    /*!
     * \brief Изменяет значение кол-ва удаленных строк
     * \param value - значение, на которое меняем.
     */
    void changeRowsDeleted(const int value);


    QJsonObject m_markerObj;            ///< Объект с данными маркера

    GraphicCommon::MarkerElement *m_marker {nullptr};

    QLabel* m_lblRowsDel {nullptr};

    int m_rowsDeleted {0};
};

#endif // TRENTETRISEXECUTE_H
