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

    // Устанавливает диапазон коодинат
    void setDiap(int diap);

    // Добавление точки
    void add(double x, double y);
    // Очистка
    void clear();

private:
    void updateItem();

    int m_diap;
    double m_prop = 1;
    int m_width = 0;
    int m_height = 0;

    QList<QPointF> m_trace;
};

#endif // TRACESKG_H
