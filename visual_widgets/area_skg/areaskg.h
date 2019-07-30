#ifndef AREASKG_H
#define AREASKG_H

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class AreaSKG;
}

class GridSKG;
class TraceSKG;
class LineSKG;
class SignalResultInfo;

class AreaSKG : public QWidget
{
    Q_OBJECT

public:
    explicit AreaSKG(QWidget *parent = 0);
    ~AreaSKG();

    int diap() const;
    void setDiap(const int diap);

    /*!
     * \brief Устанавливает маркер
     * \param x и y - координаты маркера
     */
    void setMarker(const double x, const double y);

    /*!
     * \brief Переключает режим рисования следа
     * \param trace - признак рисования следа
     */
    void showTrace(const bool trace);

    /*!
     * \brief Задает сигнал для отображения в виджете при использовании в визуале
     * \param signal
     */
    void setSignal(SignalResultInfo *signal);

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

private:
    Ui::AreaSKG *ui;

    void setAreaSKG();

    QGraphicsScene* m_sceneSKG {nullptr};
    GridSKG* m_gridSKG {nullptr};
    TraceSKG* m_traceSKG {nullptr};
    LineSKG* m_lineSKG {nullptr};
    QGraphicsRectItem* m_marker {nullptr};

    int m_diap {128};
    bool m_isShowTrace {false};
    bool m_isVisibleMarker {true};
};

#endif // AREASKG_H
