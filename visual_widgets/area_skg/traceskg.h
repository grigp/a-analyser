#ifndef TRACESKG_H
#define TRACESKG_H

#include <QObject>
#include <QGraphicsItem>
#include <QList>

/*!
 * \brief Класс следа СКГ за курсором TraceSKG class
 */
class TraceSKG : public QGraphicsItem
{
public:
    TraceSKG(int diap, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

    /*!
     * \brief Устанавливает диапазон коодинат
     * \param diap - значение диапазона
     */
    void setDiap(int diap);

    /*!
     * \brief Добавление точки
     * \param x, y - координаты
     */
    void add(double x, double y);

    /*!
     * \brief Очистка
     */
    void clear();

    /*!
     * \brief Устанавливает цвет СКГ
     */
    void setColorSKG(const QColor &color) {m_colorSKG = color;}

private:
    void updateItem();

    int m_diap;
    double m_prop {1};
    int m_width {0};
    int m_height {0};

    QList<QPointF> m_trace;
    QColor m_colorSKG {Qt::darkCyan};
};

#endif // TRACESKG_H
