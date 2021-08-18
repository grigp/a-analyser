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

    /*!
     * \brief Стили прорисовки фона Style enum
     */
    enum Style
    {
          stlPlate = 0
        , stlColor
    };

    Style style() const {return m_style;}
    void setStyle(const Style style) {m_style = style;}

    QColor color() const {return m_color;}
    void setColor(const QColor color) {m_color = color;}

    QColor frameColor() const {return m_frameColor;}
    void setFrameColor(const QColor color) {m_frameColor = color;}

    void setPliteImage(const QString fn);

    void setFrameLeftImage(const QString fn);
    void setFrameRightImage(const QString fn);
    void setFrameBottomImage(const QString fn);
    void setFrameCornerLeftImage(const QString fn);
    void setFrameCornerRightImage(const QString fn);

    void setCubeImage(const QString fn);

    QColor value(const int h, const int v) const;
    void setValue(const int h, const int v, const QColor value);


private:

    void fillData();

    qreal m_left {-100};
    qreal m_top {-200};
    qreal m_width {200};
    qreal m_height {400};
    qreal m_cubeSize {20};

    qreal m_glassBorderLR {20};  ///< Границы стакана. Слева и справа
    qreal m_glassBorderB  {20};  ///< Границы стакана. Снизу

    Style m_style {stlPlate};        ///< Стили прорисовки фона
    QColor m_color {Qt::lightGray};  ///< Цвет для стиля stlColor

    QPixmap m_pixmapPlite;          ///< Картинка плиткой
    QPixmap m_pixmapFrmLeft;        ///< Картинка левой рамки
    QPixmap m_pixmapFrmRight;       ///< Картинка правой рамки
    QPixmap m_pixmapFrmBottom;      ///< Картинка нижней рамки
    QPixmap m_pixmapFrmCornerLeft;  ///< Картинка рамки левого угла
    QPixmap m_pixmapFrmCornerRight; ///< Картинка рамки правого угла
    QPixmap m_pixmapCube;           ///< Картинка кубика

    QColor m_frameColor {Qt::white};   ///< Цвет рамки

    int m_hCount {15};       ///< Кол-во кубиков по горизонтали
    int m_vCount {21};       ///< Кол-во кубиков по вертикали

    QVector<QVector<QColor>> m_data;  ///< Данные заполнения стакана
};

#endif // TETRISGLASS_H
