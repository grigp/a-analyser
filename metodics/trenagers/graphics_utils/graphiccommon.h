#ifndef GRAPHICCOMMON_H
#define GRAPHICCOMMON_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QSizeF>

/*!
 *   Содкржит общие элементы для графических построений, прежде всего в тренажерах
 */
namespace GraphicCommon {

/*!
 * \brief Режим фоновой картинки BackgroundMode enum
 */
enum BackgroundMode
{
      bkgmPicture = 0   ///< Растянутая картинка
    , bkgmPlate         ///< Замощение плиткой
    , bkgmDrawing       ///< Рисование
};


/*!
 * \brief Законы перемещения позиции MovingLaw enum
 */
enum MovingLaw
{
      mlRandomForce  ///< Со силой воздействия, меняющейся по случайному закону
    , mlRightToLeft  ///< Справа налево
    , mlLeftToRight  ///< Слева направо
    , mlUpToDown     ///< Сверху вниз
    , mlDownToUp     ///< Снизу вверх
};

/*!
 * \brief Типы элементов игры ElementStyle enum
 */
enum ElementStyle
{
      esPictureFixed  ///< Картинка фиксированная
    , esPictureRandom ///< Картинка из набора вариантов
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
 * \brief Режим процесса удаления элемента DoneProcess enum
 */
enum DoneProcess
{
      dpHide   ///< Спрятать
    , dpShow   ///< Оставить видимым
    , dpBang   ///< Нарисовать взрыв
};


/*!
 * \brief Класс элемента - фона графической зоны BackgroundElement class
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

    ///< Работа со временем показа элемента
    double timeCounter() const {return m_timeCounter;}
    void incTimeCounter(const double delta) {m_timeCounter += delta;}
    void resetTimeCounter() {m_timeCounter = 0;}
    int presentTime() const {return m_presentTime;}

    ///< Перемещение элемента
    void setSpeed(const double vx, const double vy) {m_vx = vx; m_vy = vy;}
    double vx() const {return m_vx;}
    double vy() const {return m_vy;}

private:
    GameElementInfo* m_element {nullptr};
    QPixmap m_pixmap;
    bool m_isProcessed {false};   ///< Обработан ли элемент. Если обработан, то он не фиксируется
    QSizeF m_size;
    int m_code;
    int m_zoneIdx {-1};
    int m_presentTime {0};     ///< Время показа. Расчитывается как случайное в диапазоне m_element->timeLimitMin -:- m_element->timeLimitMax
                               ///< Если m_element->timeLimitMin > 0 и m_element->timeLimitMax > 0
    double m_timeCounter {0};  ///< Счетчик времени показа  в отсчетах (если m_presentTime > 0)

    double m_vx {0};           ///< Скорость перемещения элемента по x и y
    double m_vy {0};
};


/*!
 * \brief Класс элемента - маркера MarkerElement class
 */
class MarkerElement : public QGraphicsPixmapItem
{
public:
    explicit MarkerElement(const QPixmap &pixmap, QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(pixmap, parent) {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setShotTrace(const bool isShotTrace);

private:
    bool m_isShotTrace {false};
    int m_traceCounter {0};
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
    int timeLimitMin {0};                 ///< Минимальное время показа элемента
    int timeLimitMax {0};                 ///< Максимальное время показа элемента

    bool isMobile {false};                 ///< Перемещается ли автоматически
    MovingLaw movingLaw {mlRandomForce};   ///< Закон перемещения для isMobile == true
    int movingMaxForce {0};                ///< Маскимальная сила воздействия для isMobile == true и movingLaw == mlRandomForce
    int movingMaxSpeed {0};                ///< Максимальная скорость для isMobile == true

    DoneProcess doneProcess {dpHide};      ///< Процесс освобождения элементов

    GameElementInfo() {}
};

}



#endif // GRAPHICCOMMON_H
