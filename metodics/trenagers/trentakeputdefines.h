#ifndef TRENTAKEPUTDEFINES_H
#define TRENTAKEPUTDEFINES_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QSizeF>

namespace TrenTakePutDefines
{

/*!
 * \brief Этапы игры GameStage enum
 */
enum GameStage
{
      gsTake = 0     ///< Поиск захвата
    , gsTakeProcess  ///< Процесс захвата
    , gsPut          ///< Поиск укладки
    , gsPutProcess   ///< Процесс укладки
};

/*!
 * \brief Типы позиции зоны PositionKind enum
 */
enum PositionKind
{
      pkRandom  ///< Случайная
    , pkFixed   ///< Фиксированная
};

/*!
 * \brief Законы перемещения позиции MovingLaw enum
 */
enum MovingLaw
{
    mlRandomSpeed  ///< Со скоростью, меняющейся по случайному закону
};

/*!
 * \brief Типы элементов игры ElementStyle enum
 */
enum ElementStyle
{
      esPictureFixed  ///< Картинка фиксированная
    , esPicturePair   ///< Картинка парная захват - укладка
    , esPictureSplit  ///< Картинка разделяемая на части в зонах захвата
    , esDrawing       ///< Рисованный элемент
};

/*!
 * \brief Типы рисуемых элементов ElementDrawing enum
 */
enum ElementDrawing
{
      edRectangle  ///< Прямоугольник
    , edCircle     ///< Круг
};

/*!
 * \brief Порядок заполнения и захвата элементов
 */
enum TakeOrder
{
      toEnabledPrimary  ///< Сначала допустимые, потом остальные
                        ///<  1.1. Проходим по всем элементам enabled == true и расставляем их на зону по случайному закону
                        ///<  1.2. Проходим по всем элементам enabled == false и расставляем их по случайному закону на свободные зоны
                        ///<  1.3. Повторяем 1.2. пока есть пустые зоны
    , toAllByOrder      ///< Все по порядку
                        ///<  Если кол-во зон равно кол-ву элементов
                        ///<  Проходим по всем элементам. Для каждого элемента по случайному закону назначаем пустую зону.
};

/*!
 * \brief Режим прохождения этапов StageMode enum
 */
enum StageMode
{
      smTakePut       ///< Этап для одного элемента (взял - положил)
    , smAllElements  ///< Этап для всех элементов (все enabled элементы надо положить в свои зоны)
};

/*!
 * \brief Режим фоновой картинки BackgroundMode enum
 */
enum BackgroundMode
{
      bkgmPicture = 0   ///< Растянутая картинка
    , bkgmPlate         ///< Замощение плиткой
    , bkgmDrawing       ///< Рисование
};

struct GameElementInfo;

/*!
 * \brief Элемент графической зоны, связанный с игровым элементм в описании GameElement class
 */
class GameElement : public QGraphicsItem
{
public:
    explicit GameElement(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    GameElementInfo* elementInfo() const {return m_element;}
    void assignElementInfo(GameElementInfo* element, const QPixmap *pixmap);
    void clearElementInfo() {m_element = nullptr;}

    bool isProcessed() const {return m_isProcessed;}
    void setProcessed(const bool isProcessed) {m_isProcessed = isProcessed;}

    QSizeF size() const {return m_size;}
    void setSize(const QSizeF &size) {m_size = size;}
    void setSize(const double w, const double h)
    {
        QSizeF size(w, h);
        m_size = size;
    }

    int code() const;
    void setCode(const int code) {m_code = code;}

    int zoneIdx() const {return m_zoneIdx;}
    void setZoneIdx(const int idx) {m_zoneIdx = idx;}

private:
    GameElementInfo* m_element {nullptr};
    QPixmap m_pixmap;
    bool m_isProcessed {false};
    QSizeF m_size;
    int m_code;
    int m_zoneIdx {-1};
};

/*!
 * \brief Элемент - фон графической зоны BackgroundElement class
 */
class BackgroundElement : public QGraphicsItem
{
public:
    explicit BackgroundElement(const BackgroundMode mode, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void assignPixmap(const QString &fnPixmap);

    void setRect(const QRectF &rect) {m_rect = rect;}

private:
    QRectF m_rect;
    BackgroundMode m_mode;
    QPixmap m_pixmap;
};

/*!
 * \brief Структура информации о зоне захвата или укладки GameZoneInfo struct
 */
struct GameZoneInfo
{
public:
    PositionKind posKind {pkFixed};
    int x_min {0};                         ///< Диапазон значений позиции для posKind == pkRandom
    int x_max {0};
    int y_min {0};
    int y_max {0};
    int x {0};                             ///< Позиция для posKind == pkFixed
    int y {0};
    int width {50};                        ///< Размеры
    int height {50};
    bool isMobile {false};                 ///< Перемещается ли автоматически
    MovingLaw movingLaw {mlRandomSpeed};   ///< Закон перемещения для isMobile == true
    int movingSpeed {0};                   ///< Маскимальная скорость перемещения для isMobile == true
    int position {0};                      ///< Для стиля picture_split код позиции. Первая цифра - номер столбца, вторые две - номер строки
                                           ///< Теоретически 100 столбцов и 100 строк pos / 100 - столбец, pos % 100 - строка

    GameElement* element {nullptr};        ///< Назначенный для зоны графический элемент

    GameZoneInfo() {}

    void setElement(GameElement* elem) {element = elem;}
    void clearElement() {element = nullptr;}
};

/*!
 * \brief Структура информации об элементе GameElementInfo struct
 */
struct GameElementInfo
{
    ElementStyle style {esPictureFixed};
    QString images {""};                  ///< Для style == esPictureFixed - имя файла картинки,
                                          ///< для style == esPicturePair или esPictureSplit - папка с картинками
    bool isRepaint {false};               ///< Будет ли картинка пееркрашиваться с заданным цветом
    QColor color {Qt::yellow};            ///< Цвет перекраски или рисования в стиле style == esDrawing
    ElementDrawing drawing {edRectangle}; ///< Режим прорисовки при style == esDrawing
    bool enabled {true};                  ///< Можно ли захватывать элемент
    int code {0};                         ///< Код элемента. Можно укладывать друг на друга только элементы с совпадающим кодом
    bool movableWithMarker {false};       ///< Будет ли элемент перемещатся маркером

    GameElementInfo() {}
};

}

#endif // TRENTAKEPUTDEFINES_H
