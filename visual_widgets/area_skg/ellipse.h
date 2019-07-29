#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QObject>
#include <QGraphicsItem>

class Ellipse : public QGraphicsItem
{
public:
    Ellipse(QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

    /*!
     * \brief Устанавливает диапазон коодинат
     * \param diap - значение дипазона
     */
    void setDiap(int diap);

    /*!
     * \brief Устанавливает параметры эллипса
     * \param centerX, centerY - центр
     * \param sizeA, sizeB - длины осей
     * \param angle - угол наклона
     */
    void setEllipse(const double centerX, const double centerY,
                    const double sizeA, const double sizeB, const double angle);

private:

    int m_diap {128};
    double m_prop {1};
    int m_width {0};
    int m_height {0};

    double m_centerX {0};
    double m_centerY {0};
    double m_sizeA {0};
    double m_sizeB {0};
    double m_angle {0};

    double m_offsX {0}, m_offsY {0};
};

#endif // ELLIPSE_H
