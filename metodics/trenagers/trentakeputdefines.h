#ifndef TRENTAKEPUTDEFINES_H
#define TRENTAKEPUTDEFINES_H

#include <QGraphicsItem>

namespace TrenTakePutDefines
{

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

struct GameElementInfo;

/*!
 * \brief Элемент графической зоны, связанный с игровым элементм в описании GameElement class
 */
class GameElement : public QGraphicsItem
{
public:
    explicit GameElement(QGraphicsItem *parent = nullptr) : QGraphicsItem(parent) {}

    GameElementInfo* elementInfo() const {return m_element;}
    void assignElementInfo(GameElementInfo* element) {m_element = element;}

private:
    GameElementInfo* m_element {nullptr};
};

/*!
 * \brief Структура информации о зоне захвата или укладки GameZoneInfo struct
 */
struct GameZoneInfo
{
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

    GameElement* element {nullptr};        ///< Назначенный для зоны графический элемент

    GameZoneInfo() {}
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

    GameElementInfo() {}
};

}

#endif // TRENTAKEPUTDEFINES_H
