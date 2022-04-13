#ifndef GRIDSKG_H
#define GRIDSKG_H

#include <QObject>
#include <QGraphicsItem>

class GridSKG : public QGraphicsItem
{
public:
    GridSKG(int diap, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

    // Устанавливает диапазон коодинат
    void setDiap(int diap);

    // Возвращает размер поля для вывода меток
    int getLabelSpace();

private:
    // Прорисовка позиции сетки на позиции posGrid
    void drawPositionGrid(QPainter* painter, int left, int top, int right, int bottom, int posGrid, bool isLabels);

    int m_diap;
    double m_prop {1};
    int m_width {0};
    int m_height {0};
};

#endif // GRIDSKG_H
