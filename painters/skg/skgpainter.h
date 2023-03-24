#ifndef SKGPAINTER_H
#define SKGPAINTER_H

#include <QObject>
#include <QPainter>
#include <QGraphicsScene>
#include <QList>

#include "skgdefines.h"

class SignalAccess;

/*!
 * \brief Класс рисователя статокинезиграммы The SKGPainter class
 */
class SKGPainter
{
public:
    explicit SKGPainter(QPainter* painter, QRect geometry);
    explicit SKGPainter();
    virtual ~SKGPainter();

    /*!
     * \brief Задает канву прорисовки. Будет испльзоваться в режиме виджета для задания
     * \param painter - рисователь
     * \param geometry - размер области прорисовки
     */
    void setCanvas(QPainter* painter, QRect geometry);

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
     * \brief Устанавливает размер маркера
     * \param size - размер маркера
     */
    void setMarkerSize(const int size);

    /*!
     * \brief Переключает режим рисования следа
     * \param trace - признак рисования следа
     */
    void showTrace(const bool trace);

    /*!
     * \brief Задает сигнал для отображения в виджете при использовании в визуале
     * \param signal
     * \param num - номер СКГ
     * \param begin, end - границы участка
     */
    void setSignal(SignalAccess *signal, const int num = 0, const int begin = -1, const int end = -1);

    /*!
     * \brief Задает границы участка сигнала
     * \param begin, end - границы участка
     * \param num - номер СКГ
     */
    void setSection(const int begin, const int end, const int num = 0);

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
     * \brief Установка смещения снаружи
     * \param offsetX, offsetY - смещения
     * \param num - номер СКГ
     */
    void setOffset(const double offsetX, const double offsetY, const int num = 0);

    /*!
     * \brief Устанавливает параметры эллипса
     * \param sizeA, sizeB - длины осей
     * \param angle - угол наклона
     */
    void setEllipse(const double sizeA, const double sizeB, const double angle);

    /*!
     * \brief Устанавливает цвет СКГ
     */
    void setColorSKG(const QColor &color, const int num = 0);
    QColor colorSKG(const int num = 0) const;

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
    int addBrokenLine(SKGDefines::BrokenLine &bl);

    /*!
     * \brief Удаляет ломаную по индексу
     * \param idx - индекс ломаной
     * \return true, если удачно
     */
    bool deleteBrokenLine(const int idx);

    /*!
     * \brief Устанавливает видимость СКГ
     * \param isVisible - будет ли видна
     * \param num - номер СКГ
     */
    void setVisibleSKG(const bool isVisible, const int num = 0);

    /*!
     * \brief Добавляет отображаемую платформу
     * \param platform - положение и размер платформу
     */
    void addPlatform(QRect platform);

    /*!
     * \brief Процедура прорисовки на рисователе m_painter  в рамках m_geometry
     */
    void doPaint(const double ratio);

protected:
    virtual void doUpdate();

private:
    QPainter* m_painter {nullptr};
    QRect m_geometry {QRect(0, 0, 0, 0)};

    /*!
     * \brief Прорисовка сетки
     */
    void drawGrid(const double ratio);

    /*!
     * \brief Прорисовывает позиции сетки на позиции posGrid
     * \param posGrid
     * \param isLabels
     */
    void drawPositionGrid(int posGrid, bool isLabels);

    /*!
     * \brief Прорисовывает статокинезиграммы
     */
    void drawSKG();

    /*!
     * \brief Прорисовывает платформы
     */
    void drawPlatforms(const double ratio);

    /*!
     * \brief Прорисовывает ломаные
     */
    void drawBrokenLines(const double ratio);

    /*!
     * \brief Прорисовывает маркер
     */
    void drawMarker();

    struct TargetInfo
    {
        QGraphicsItem* item;
        QPointF pos;
        TargetInfo(QGraphicsItem* itm, QPointF pt)
            : item(itm), pos(pt) {}
    };

    /*!
     * \brief Структура данных, содержащая сигнал и его параметры The SignalData struct
     */
    struct SignalData
    {
        SignalAccess* signal {nullptr};
        QColor color {Qt::darkCyan};
        int begin {-1};
        int end {-1};
        double offsX {0}, offsY {0};
        bool visible {true};
        SignalData(SignalAccess* sig, const QColor col, const int b, const int e);
//        SignalData(SignalAccess* sig, const QColor col, const int b, const int e)
//            : signal(sig), color(col), begin(b), end(e) {}
        //! Рассчитывает параметры сигнала
        void assignSignal(SignalAccess* sig = nullptr);
    };

//    TraceSKG* m_traceSKG {nullptr};
//    QList<TargetInfo> m_targets;

    QColor m_markerColor {Qt::red};
    QColor m_markerBorderColor {Qt::darkRed};
    QColor m_platformsColor {QColor(235, 235, 235)};
    QColor m_ellipseColor {Qt::darkBlue};

    int m_diap {128};
    bool m_isShowTrace {false};
    bool m_isVisibleMarker {true};

    double m_prop {1};
    int m_width {0};
    int m_height {0};
    int m_midX {0};
    int m_midY {0};
    int m_left {0};
    int m_top {0};
    int m_right {0};
    int m_bottom {0};
    int m_space {0};
    double m_ratio {1};

    QList<SignalData> m_signals;
    bool m_isZeroing {false};

    QList<QRect> m_platforms;

    ///< Параметры эллипса
    double m_sizeA {0};
    double m_sizeB {0};
    double m_angle {0};

    QList<SKGDefines::BrokenLine> m_brokenLines;   ///< Список ломаных для построения

    ///< Координаты маркера
    double m_mx {0};
    double m_my {0};
    int m_mSize {10};
};

#endif // SKGPAINTER_H
