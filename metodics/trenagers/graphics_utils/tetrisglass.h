#ifndef TETRISGLASS_H
#define TETRISGLASS_H

#include <QObject>
#include <QGraphicsItem>

/*!
 * \brief Класс, реализующий "Стакан" в тетрисе TetrisGlass class
 */
class TetrisGlass : public QGraphicsItem
{
public:
    explicit TetrisGlass(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /*!
     * \brief Рассчитывает размеры стакана в координатах игровой сцены
     * \param sceneWidth, sceneHeight - размеры игровой сцены
     * \param h - кол-во позиций по горизонтали
     * \param v - кол-во позиций по вертикали
     */
    void setGeometry(const qreal sceneWidth, const qreal sceneHeight, const int h, const int v);

//    /*!
//     * \brief Устанавливает размеры стакана в координатах игровой сцены
//     */
//    void setGeometry(const qreal left, const qreal top, const qreal width, const qreal height);

private:
    qreal m_left {-100};
    qreal m_top {-200};
    qreal m_width {200};
    qreal m_height {400};

    qreal m_glassBorder {20};  ///< Границы стакана. Слева, снизу и справа
};

#endif // TETRISGLASS_H
