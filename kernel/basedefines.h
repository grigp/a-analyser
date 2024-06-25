#ifndef BASEDEFINES_H
#define BASEDEFINES_H

#include <QObject>
#include <QMap>
#include <QCoreApplication>

namespace BaseDefines {

static const int NegativeValue = -1;
static const int ZeroValue = 0;
static const int PositiveValue = 1;

/*!
 * \brief Константы сторон Side enum
 */
enum Side
{
      NoSide = 0
    , Left
    , Right
    , Up
    , Down
};

/*!
 * \brief Коды направлений Directions enum
 * Нумеруются по часовой стрелке
 */
enum Directions
{
      dirNone = -1
    , dirUp = 0
    , dirRight
    , dirDown
    , dirLeft
};

static QMap<QString, Directions> DirectionValueIndex {
    std::pair<QString, Directions> ("up", dirUp)
  , std::pair<QString, Directions> ("right", dirRight)
  , std::pair<QString, Directions> ("down", dirDown)
  , std::pair<QString, Directions> ("left", dirLeft)
};

static QMap<Directions, QString> DirectionValueUIDName {
    std::pair<Directions, QString> (dirUp, "up")
  , std::pair<Directions, QString> (dirRight, "right")
  , std::pair<Directions, QString> (dirDown, "down")
  , std::pair<Directions, QString> (dirLeft, "left")
};

static QMap<Directions, QString> DirectionValueFBName {
    std::pair<Directions, QString> (dirUp, QCoreApplication::tr("вперед"))
  , std::pair<Directions, QString> (dirRight, QCoreApplication::tr("вправо"))
  , std::pair<Directions, QString> (dirDown, QCoreApplication::tr("назад"))
  , std::pair<Directions, QString> (dirLeft, QCoreApplication::tr("влево"))
};

static QMap<Directions, QString> DirectionValueUDName {
    std::pair<Directions, QString> (dirUp, QCoreApplication::tr("вверх"))
  , std::pair<Directions, QString> (dirRight, QCoreApplication::tr("вправо"))
  , std::pair<Directions, QString> (dirDown, QCoreApplication::tr("вниз"))
  , std::pair<Directions, QString> (dirLeft, QCoreApplication::tr("влево"))
};


/*!
 * \brief Выбрать режима смены направления DirectionMode enum
 */
enum DirectionMode
{
      dmRandom = 0         ///< По случайному закону
    , dmClockwise          ///< По часовой стрелке
    , dmCounterClockwise   ///< Против часовой стрелке
};

static QMap<QString, DirectionMode> DirectionModeValueIndex {
    std::pair<QString, DirectionMode> ("random", dmRandom)
  , std::pair<QString, DirectionMode> ("clockwise", dmClockwise)
  , std::pair<QString, DirectionMode> ("counter_clockwise", dmCounterClockwise)
};

static QMap<DirectionMode, QString> DirectionModeValueName {
    std::pair<DirectionMode, QString> (dmRandom, "random")
  , std::pair<DirectionMode, QString> (dmClockwise, "clockwise")
  , std::pair<DirectionMode, QString> (dmCounterClockwise, "counter_clockwise")
};

/*!
 * \brief Режим фиксации этапа The StageFinishMode enum
 * Через заданное время
 * При достижении цели
 */
enum StageFinishMode
{
      sfmFixedTime = 0   ///< Заданное время
    , sfmFixingOnTarget  ///< Удержание на цели
};

static QMap<QString, StageFinishMode> StageFinishModeValueIndex {
    std::pair<QString, StageFinishMode> ("fixed_time", sfmFixedTime)
  , std::pair<QString, StageFinishMode> ("fixing_on_target", sfmFixingOnTarget)
};

static QMap<StageFinishMode, QString> StageFinishModeValueName {
    std::pair<StageFinishMode, QString> (sfmFixedTime, "fixed_time")
  , std::pair<StageFinishMode, QString> (sfmFixingOnTarget, "fixing_on_target")
};


/*!
 * \brief Выбрать кольцевое правило обхода CirceRoundRuleMode enum
 */
enum CirceRoundRuleMode
{
      crmRadial = 0         ///< Радиальное
    , crmCircle             ///< Кольцевое
};

static QMap<QString, CirceRoundRuleMode> CirceRoundRuleModeValueIndex {
    std::pair<QString, CirceRoundRuleMode> ("radial", crmRadial)
  , std::pair<QString, CirceRoundRuleMode> ("circle", crmCircle)
};

static QMap<CirceRoundRuleMode, QString> CirceRoundRuleModeValueName {
    std::pair<CirceRoundRuleMode, QString> (crmRadial, "radial")
  , std::pair<CirceRoundRuleMode, QString> (crmCircle, "circle")
};

/*!
 * \brief Коды направлений Directions enum
 * Нумеруются по часовой стрелке
 */
enum TriangleCorner
{
      tcNone = -1
    , tcTop = 0
    , tcLeftDown
    , tcRightDown
};

static QMap<QString, TriangleCorner> TriangleCornerValueIndex {
    std::pair<QString, TriangleCorner> ("top", tcTop)
  , std::pair<QString, TriangleCorner> ("left_down", tcLeftDown)
  , std::pair<QString, TriangleCorner> ("right_down", tcRightDown)
};

static QMap<TriangleCorner, QString> TriangleCornerValueUIDName {
    std::pair<TriangleCorner, QString> (tcTop, "top")
  , std::pair<TriangleCorner, QString> (tcLeftDown, "left_down")
  , std::pair<TriangleCorner, QString> (tcRightDown, "right_down")
};

static QMap<TriangleCorner, QString> TriangleCornerValueName {
    std::pair<TriangleCorner, QString> (tcTop, QCoreApplication::tr("верхняя"))
  , std::pair<TriangleCorner, QString> (tcLeftDown, QCoreApplication::tr("левая"))
  , std::pair<TriangleCorner, QString> (tcRightDown, QCoreApplication::tr("правая"))
};


///< Границы участка сигнала
struct Section
{
    int begin;   ///< Начало
    int end;     ///< Конец
    Section(int b, int e) : begin(b), end(e) {}
};

///< Тип экстремума
enum ExtremumKind
{
      Minimum = 0
    , Maximum
};

///< Структура описания экстремума
struct Extremum
{
    int pos;             ///< Точка
    ExtremumKind kind;   ///< Тип
    Extremum(int p, ExtremumKind k) : pos(p), kind(k) {}
};


/*!
 * \brief Структура группировки пары показателей FctTblPair struct
 */
struct FctTblPair
{
    QString first;
    QString second;
    FctTblPair(const QString f, const QString s)
        : first(f), second(s)
    {}
};

/*!
 * \brief Структура группировки минимального и максимального значений The MinMaxValue struct
 */
struct MinMaxValue
{
    double min;
    double max;
    MinMaxValue(const double mn, const double mx)
        : min(mn), max(mx)
    {}
};

///< Константы границ зон коррекций
static const double ZoneMotorLo = 0.1;   ///< dZone100160Min
static const double ZoneMotorHi = 0.16;  ///< dZone100160Max
static const double ZoneKognLo = 0.2;    ///< dZone200280Min
static const double ZoneKognHi = 0.28;   ///< dZone200280Max

/*!
 * \brief Уровень объекта в тесте: визуализатора, мультифактора The TestLevel enum
 */
enum TestLevel
{
      tlNone = 0    ///< Не задан
    , tlBase        ///< Нескольких тестов
    , tlTest        ///< Теста
    , tlProbe       ///< Пробы
    , tlChannel     ///< Канала
    , tlSection     ///< Секции сигнала
    , tlFragment    ///< Фрагмента сигнала (на будущее)
};

/*!
 * \brief режимы проведения тестов The RunningMode enum
 */
enum RunningMode
{
      rmOperator = 0  ///< С оператором
    , rmAutomatic     ///< Автоматический
};

/*!
 * \brief Что делать при двойном нажатии на запись теста The TestDoubleClickMode enum
 */
enum TestDoubleClickMode
{
      tdcmEditParams = 0   ///< Редактировать параметры
    , tdcmRunTest          ///< Запустить тест
};

/*!
 * \brief Информация о цели. Координаты и видимость The TargetInfo struct
 */
struct TargetInfo
{
    double x;
    double y;
    bool visible;
    TargetInfo() {}
    TargetInfo(std::tuple<double, double, bool> params)
        : x(std::get<0>(params))
        , y(std::get<1>(params))
        , visible(std::get<2>(params))
    {}
};


}

#endif // BASEDEFINES_H
