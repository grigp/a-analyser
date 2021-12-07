#ifndef BASEUTILS_H
#define BASEUTILS_H

#include <QObject>
#include <QPainter>
#include <QAbstractItemModel>
#include <QCoreApplication>

namespace BaseUtils
{

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

static QMap<Directions, QString> DirectionValueName {
    std::pair<Directions, QString> (dirUp, QCoreApplication::tr("вперед"))
  , std::pair<Directions, QString> (dirRight, QCoreApplication::tr("вправо"))
  , std::pair<Directions, QString> (dirDown, QCoreApplication::tr("назад"))
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
    std::pair<QString, DirectionMode> ("return", dmRandom)
  , std::pair<QString, DirectionMode> ("clockwise", dmClockwise)
  , std::pair<QString, DirectionMode> ("counter_clockwise", dmCounterClockwise)
};

static QMap<DirectionMode, QString> DirectionModeValueName {
    std::pair<DirectionMode, QString> (dmRandom, "return")
  , std::pair<DirectionMode, QString> (dmClockwise, "clockwise")
  , std::pair<DirectionMode, QString> (dmCounterClockwise, "counter_clockwise")
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
 * \brief Возвращает время в формате hh:mm:ss по кол-ву секунд
 * \param secCnt - кол-во секунд
 * \param isHour - показывать ли часы
 */
QString getTimeBySecCount(const int secCnt, const bool isHour = false);

/*!
 * \brief Возвращает имя файла с данными для переводчика и строковый код языка
 * \param fileName - имя файла с данными для переводчика
 * \param langCode - строковый код языка
 * \return true, если найден файл и false, если нет
 */
bool getTranslatorFileName(QString &fileName, QString &langCode);

/*!
 * \brief Возвращает угол в полярной системе координат (0 - 2 * M_PI) по значениям координат в декартовой системе координат
 * \param x, y - координаты в декартовой системе координат
 */
double getAngleByCoord(const double x, const double y);

/*!
 * \brief Возвращает разницу между двумя углами a1 - a2
 * Исходные углы и разница в радианах
 * \param a1, a2 - исходные углы
 */
double getDifferenceAngles(const double a1, const double a2);

/*!
 * \brief Возвращает размер текста для установленных параметров рисователя
 * \param painter - указатель на рисователь
 * \param text - текст
 */
QSize getTextSize(QPainter *painter, const QString &text);

/*!
 * \brief Преобразует цвет в строку, содержащуу состоявляющие цвета: "aarrggbb"
 * \param color - цвет
 */
QString colorToRGBAStr(const QColor color);

/*!
 * \brief Преобразует строку, содержащуу состоявляющие цвета: "aarrggbb"  в цвет QColor
 * \param rgbaStr - строка в формате "aarrggbb"
 */
QColor strRGBAToColor(const QString rgbaStr);

/*!
 * \brief Перекрашивает картинку в заданный цвет
 * \param pixmap - картинка
 * \param color - заданный цвет
 */
void setColoredPicture(QPixmap &pixmap, const QColor &color);

/*!
 * \brief Записывает данные табличной модели в файл для последующего открытия в MS Excel или OO Calc
 * Данные сохраняются в виде csv файла.
 * CSV (от англ. Comma-Separated Values — значения, разделённые запятыми) — текстовый формат,
 * предназначенный для представления табличных данных.
 * Каждая строка файла — это одна строка таблицы. Значения отдельных колонок разделяются разделительным
 * символом (delimiter) — запятой ( , ).
 * Однако, большинство программ вольно трактует стандарт CSV и допускают использование иных символов
 * в качестве разделителя.
 * \param model - указатель на модель
 * \param fileName - имя файла csv
 */
void modelToMSExcel(const QAbstractItemModel* model, const QString &fileName);

/*!
 * \brief modelToText Записывает данные табличной модели в текстовый файл
 * \param model - указатель на модель
 * \param fileName - имя файла csv
 */
void modelToText(const QAbstractItemModel* model, const QString &fileName);

/*!
 * \brief Сохраняет данные вектора в текстовый файл
 * \param vector - данные вектора
 * \param fileName - имя текстового файла
 */
void vectorToText(QVector<double>& vector, const QString &fileName);

/*!
 * \brief Инвертирует вектор, заменяя хвост на начало
 * \param vector - вектор
 */
void swapVector(QVector<double>& vector);

/*!
 * \brief Типы фильтров ФНЧ FilterKind enum
 */
enum FilterKind
{
      fkCriticalAttenuation = 0  ///< Критического затухания
    , fkBessel                   ///< Бесселя
    , fkBatterwort               ///< Баттерворта
    , fkChebyshev                ///< Чебышева с неравн. 0,5 дБ
};

/*!
 * \brief Фильтр низкой частоты. Тип фильтра 2-го порядка табл. 13.6 стр. 194
 * \param buffer - фильтруемый массив
 * \param fd - частота дискретизации
 * \param fc - частота среза
 * \param tf - тип фиьтра
 * \param n1, n2 - границы
 */
void filterLowFreq(QVector<double> &buffer,
                   const double fd,
                   const double fc,
                   const FilterKind tf,
                   const int n1,
                   const int n2);

/*!
 * \brief Возвращает знак числа number
 * Если number > 0 возвразщает 1
 * Если number < 0 возвразщает -1
 */
int sign(const double number);

/*!
 * \brief Преобразует коодинаты из декартовой ситемы в полярную
 * \param x, y - координаты в декартовой системе
 * \param r - радиус
 * \param ph - угол
 */
void convertDecartToPolar ( const double x, const double y, double &r, double &ph);

/*!
 * \brief Возвращает текст резюме к преобладанию коррекций
 * \param cdv - значение параметра преобладания
 * \param resume - возвращаемый текст резюме
 * \param resumeColor - цвет текста резюме
 */
void setCorrectionsDominanceResume(const double cdv, QString &resume, QString &resumeColor);

/*!
 * \brief Возвращает текст резюме к опережению маркера
 * \param orv - значение параметра преобладания
 * \param resume - возвращаемый текст резюме
 * \param resumeColor - цвет текста резюме
 */
void setOutrunningResume(const double orv, QString &resume, QString &resumeColor);

/*!
 * \brief Возвращает масштабный множитель по индексу масштаба
 * \param idx - индекс масштаба 1, 2, 3, ...
 * \return 1, 2, 4, 8, 16, 32, ....
 */
int scaleMultiplier(const int idx);


/*!
 * \brief Класс, рассчитывающий математическое ожидание и стандартное отклонение с итеративным добавлением MidAndStandardDeviation class
 */
class MidAndStandardDeviation
{
public:
    MidAndStandardDeviation() {clear();}

    /*!
     * \brief Добавляет значение в список
     */
    void add(const double value) {m_values.append(value);}

    /*!
     * \brief Расчет
     * \param mid - возвращает среднее арифметическое
     * \param stdDev - возвращает стандартное отклонение
     */
    void calculate(double& mid, double& stdDev) const;

    /*!
     * \brief очищает список значений
     */
    void clear() {m_values.clear();}

    int count() const {return m_values.size();}

private:
    QVector<double> m_values;
};

}


#endif // BASEUTILS_H
