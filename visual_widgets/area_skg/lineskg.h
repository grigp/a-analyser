#ifndef LINESKG_H
#define LINESKG_H

#include <QObject>
#include <QGraphicsItem>

class SignalAccess;

/*!
 * \brief Класс линии СКГ LineSKG class
 * Грузится один раз и полностью
 */
class LineSKG : public QGraphicsItem
{
public:
    LineSKG(int diap, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

    // Устанавливает диапазон коодинат
    void setDiap(int diap);

    /*!
     * \brief Задает сигнал для отображения в виджете при использовании в визуале
     * \param signal
     */
    void setSignal(SignalAccess *signal);

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
    void updateItem();

    int m_diap;
    double m_prop {1};
    int m_width {0};
    int m_height {0};
    double m_offsX {0}, m_offsY {0};
    bool m_isZeroing {false};

    SignalAccess *m_signal {nullptr};

    ///< Параметры эллипса
    double m_sizeA {0};
    double m_sizeB {0};
    double m_angle {0};

};

#endif // LINESKG_H
