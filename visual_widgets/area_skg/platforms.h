#ifndef PLATFORMS_H
#define PLATFORMS_H

#include <QObject>
#include <QGraphicsItem>

/*!
 * \brief Класс графического элемента, показывающего платформы The Platforms class
 */
class Platforms : public QGraphicsItem
{
public:
    Platforms(int diap, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

    /*!
     * \brief Устанавливает диапазон коодинат
     * \param diap - диапазон
     */
    void setDiap(int diap);

    /*!
     * \brief Добавляет отображаемую платформу
     * \param platform - положение и размер платформу
     */
    void addPlatform(QRect platform);

    void setColorPlatform(QColor color);
    void setColorFrame(QColor color);

private:
    int m_diap {128};
    double m_prop {1};
    int m_width {0};
    int m_height {0};
    QList<QRect> m_platforms;

    QColor m_color {Qt::gray};
    QColor m_colorFrame {Qt::darkGray};
};

#endif // PLATFORMS_H
