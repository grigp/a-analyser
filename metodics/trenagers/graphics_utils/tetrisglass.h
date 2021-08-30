#ifndef TETRISGLASS_H
#define TETRISGLASS_H

#include <QObject>
#include <QGraphicsItem>
#include <QMap>
#include <QPixmap>

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

    /*!
     * \brief Доступ к цвтеам в ячейках Qt::black - ячейка пустая
     * \param h, v - позиции по горизонтали и вертикали
     */
    QColor value(const int h, const int v) const;
    void setValue(const int h, const int v, const QColor value);

    /*!
     * \brief Добавляет цвет в список допустимых
     * \param color - цвет
     */
    void addColor(const QColor color);

    /*!
     * \brief Пересоздает фигуру на сцене
     * \param figure - двумерный массив данных фигуры
     */
    void setNewFigure(QVector<QVector<QColor>> figure);

    /*!
     * \brief Поворачивает фигуру на 90 градусов по часовой стрелке
     */
    void rotateFigure();

    /*!
     * \brief Возвращает прямоугольник положение фигуры в координатах сцены
     */
    QRectF getFigureRect() const;

    /*!
     * \brief Возвращает прямоугольник позицию фигуры в номерах позиций в стакане
     */
    QRect getFigurePosition() const;

    /*!
     * \brief Возвращает цвет кубика фигуры в повернутом положении
     * \param h, v - номер кубика по ширине и высота
     */
    QColor getFigureColor(const int h, const int v);

    /*!
     * \brief Перемещает фигуру на позицию
     * \param x, y - координаты позиции
     * \return true, если фигура коснулась конфигурации или дна стакана и false в противном случае
     */
    bool setFigureCoordinates(const qreal x, const qreal y);

    /*!
     * \brief Возвращает позицию в стакане по координатам сцены
     * \param x, y - координаты сцены
     * \return позиция в стакане, правый нижний угол - нули
     */
    QPoint getPositionByCoord(const qreal x, const qreal y) const;

    /*!
     * \brief Возвращает прямоугольник с координатами позиции
     * \param pos - позиция в стакане, правый нижний угол - нули
     */
    QRectF getCoordinatesOfPosition(const QPoint pos) const;

    qreal borderLR() {return  m_glassBorderLR;}
    qreal borderB()  {return m_glassBorderB;}

private:

    void fillData();

    /*!
     * \brief Прорисовывает фигуру с помощью указанного рисователя
     */
    void showFigure(QPainter *painter);

    /*!
     * \brief Возвращает true, если фигура коснулась конфигурации или дна стакана
     */
    bool figureOnConfigOrBottom();

    /*!
     * \brief Возвращает true, если фигура внутри конфигурации
     */
    bool figureIntoConfig();

    /*!
     * \brief Корректирует позицию фигуры по сетке по фронтали прежде всего
     * \param ox, oy - координаты предыдущей позиции
     */
    void correctFigurePosition(const qreal tx, const qreal ty);

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

    QVector<QVector<QColor>> m_data;    ///< Данные заполнения стакана
    QMap<int, QPixmap> m_allowColors;   ///< Допустимые цвета

    QVector<QVector<QColor>> m_figure;  ///< Данные переносимой фигуры
    qreal m_figX {0};         ///< Координаты фигуры
    qreal m_figY {0};
    int m_figAngleCode {0};   ///< Код угла фигуры m_figAngleCode % 4 = 0 - 0гр., 1 - 90гр., 2 - 180гр., 4 - 270гр. Каждый поворот ++m_figAngleCode;
};

#endif // TETRISGLASS_H
