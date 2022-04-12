#ifndef AREASKG_H
#define AREASKG_H

#include <QWidget>
#include <QGraphicsScene>
#include <QList>

#include "areaskgdefines.h"

namespace Ui {
class AreaSKG;
}

class GridSKG;
class TraceSKG;
class LineSKG;
class Platforms;
class BrokenLinesSKG;
class SignalAccess;

class AreaSKG : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor skg_color READ colorSKG WRITE setColorSKG DESIGNABLE true)
    Q_PROPERTY(QColor ellipse_color READ colorEllipse WRITE setColorEllipse DESIGNABLE true)
    Q_PROPERTY(QColor marker_color READ colorMarker WRITE setColorMarker DESIGNABLE true)
    Q_PROPERTY(QColor platforms_color READ colorPlatforms WRITE setColorPlatforms DESIGNABLE true)

public:
    explicit AreaSKG(QWidget *parent = nullptr);
    ~AreaSKG();

    int diap() const;
    void setDiap(const int diap);

    /*!
     * \brief Добавляет маркер
     */
    void addMarker();

    /*!
     * \brief Устанавливает маркер
     * \param x и y - координаты маркера
     */
    void setMarker(const double x, const double y);

    /*!
     * \brief Устанавливает цвет маркера
     * \param colorBackground - цвет фона
     * \param colorBorder - цвет рамки
     */
    void setMarkerColor(const QColor colorBackground, const QColor colorBorder);

    /*!
     * \brief Переключает режим рисования следа
     * \param trace - признак рисования следа
     */
    void showTrace(const bool trace);

    /*!
     * \brief Задает сигнал для отображения в виджете при использовании в визуале
     * \param signal
     * \param begin, end - границы участка
     */
    void setSignal(SignalAccess *signal, const int begin = -1, const int end = -1);

    /*!
     * \brief Задает границы участка сигнала
     * \param begin, end - границы участка
     */
    void setSection(const int begin, const int end);

    /*!
     * \brief Управляет видимостью маркера
     * \param visibleMarker - будет ли видим маркер
     */
    void setVisibleMarker(const bool visibleMarker);

    /*!
     * \brief Устанавливает признак центровки сигнала
     */
    void setZeroing(const bool zeroing);

    /*!
     * \brief Устанавливает параметры эллипса
     * \param sizeA, sizeB - длины осей
     * \param angle - угол наклона
     */
    void setEllipse(const double sizeA, const double sizeB, const double angle);

    /*!
     * \brief Устанавливает цвет СКГ
     */
    void setColorSKG(const QColor &color);
    QColor colorSKG() const;

    /*!
     * \brief Устанавливает цвет эллипса
     */
    void setColorEllipse(const QColor &color);
    QColor colorEllipse() const;

    QColor colorMarker() const;
    void setColorMarker(const QColor &color);

    QColor colorPlatforms() const;
    void setColorPlatforms(const QColor &color);
    /*!
     * \brief Добавляет метку цели
     * \param x, y - координаты цели
     * \param colorBackground - цвет фона
     * \param colorBorder - цвет границы
     */
    void addTarget(const double x, const double y, const QColor colorBackground, const QColor colorBorder);

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
     * \brief Добавляет ломанную
     * \param bl - ломанная
     * \return индекс в списке ломаных
     */
    int addBrokenLine(AreaSKGDefines::BrokenLine &bl);

    /*!
     * \brief Удаляет ломаную по индексу
     * \param idx - индекс ломаной
     * \return true, если удачно
     */
    bool deleteBrokenLine(const int idx);

    /*!
     * \brief Устанавливает видимость СКГ
     * \param isVisible - будет ли видна
     */
    void setVisibleSKG(const bool isVisible);

    /*!
     * \brief Добавляет отображаемую платформу
     * \param platform - положение и размер платформу
     */
    void addPlatform(QRect platform);


protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::AreaSKG *ui;

    void setAreaSKG();

    QColor getFrameColor(const QColor color) const;

    struct TargetInfo
    {
        QGraphicsItem* item;
        QPointF pos;
        TargetInfo(QGraphicsItem* itm, QPointF pt)
            : item(itm), pos(pt) {}
    };

    QGraphicsScene* m_sceneSKG {nullptr};
    GridSKG* m_gridSKG {nullptr};
    TraceSKG* m_traceSKG {nullptr};
    LineSKG* m_lineSKG {nullptr};
    Platforms* m_platforms {nullptr};
    BrokenLinesSKG* m_brokenLinesSKG {nullptr};
    QGraphicsRectItem* m_marker {nullptr};
    QList<TargetInfo> m_targets;

    QColor m_markerColor {Qt::red};
    QColor m_platformsColor {Qt::gray};

    int m_diap {128};
    bool m_isShowTrace {false};
    bool m_isVisibleMarker {true};
};

#endif // AREASKG_H
