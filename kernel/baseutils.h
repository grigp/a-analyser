#ifndef BASEUTILS_H
#define BASEUTILS_H

#include <QObject>
#include <QPainter>
#include <QAbstractItemModel>
#include <QCoreApplication>

namespace BaseUtils
{


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
void modelToText(const QAbstractItemModel* model, const QString &fileName, const QString& divider = "");

/*!
 * \brief Сохраняет данные вектора в текстовый файл
 * \param vector - данные вектора
 * \param fileName - имя текстового файла
 */
void vectorToText(QVector<double>& vector, const QString &fileName);

/*!
 * \brief Сохраняет вектор точек в текстовые файлы, добавляя расширения ".x" для форнтали и ".y" для сагиттали
 * \param vector - данные вектора
 * \param fileName - имя текстового файла без расширения
 * \param separator - разделитель целой и дробной частей
 */
void pointsToTextSeparate(QVector<QPointF>& vector, const QString &fileName, const char separator);

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

static QMap<FilterKind, QString> FilterName =
{
      std::pair<FilterKind, QString> (fkCriticalAttenuation, QCoreApplication::tr("Критического затухания"))
    , std::pair<FilterKind, QString> (fkBessel, QCoreApplication::tr("Бесселя"))
    , std::pair<FilterKind, QString> (fkBatterwort, QCoreApplication::tr("Баттерворта"))
    , std::pair<FilterKind, QString> (fkChebyshev, QCoreApplication::tr("Чебышева с неравн. 0,5 дБ"))
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
void convertDecartToPolar(const double x, const double y, double &r, double &ph);

/*!
 * \brief Преобразует коодинаты из декартовой ситемы в полярную
 * \param r - радиус
 * \param ph - угол
 * \param x, y - координаты в декартовой системе
 */
void convertPolarToDecart(const double r, const double ph, double &x, double &y);

/*!
 * \brief Поворачивает точку в декартовой системе координат вокруг 0 на заданный угол
 * \param x, y - исходные координаты
 * \param alfa - угол поворота
 * \param xr, yr - выходные координаты
 */
void rotatePoint(const double x, const double y, const double alfa, double &xr, double &yr);

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
 * \brief Возвращает знак числа
 * \param value - число
 * \return BaseDefines::NegativeValue, если value < 0, BaseDefines::ZeroValue, если value == 0, BaseDefines::PositiveValue, если value > 0;
 */
int sign(const int value);

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

static QMap<QString, double> ScaleKoefitients =
{
      std::pair<QString, double> ("1/16", 0.0625)
    , std::pair<QString, double> ("1/8", 0.125)
    , std::pair<QString, double> ("1/4", 0.25)
    , std::pair<QString, double> ("1/2", 0.5)
    , std::pair<QString, double> ("1", 1)
    , std::pair<QString, double> ("2", 2)
    , std::pair<QString, double> ("4", 4)
    , std::pair<QString, double> ("8", 8)
    , std::pair<QString, double> ("16", 16)
    , std::pair<QString, double> ("32", 32)
    , std::pair<QString, double> ("64", 64)
    , std::pair<QString, double> ("128", 128)
    , std::pair<QString, double> ("256", 256)
    , std::pair<QString, double> ("512", 512)
};


static QList<QString> ScalesStab =
{
    "1/16", "1/8", "1/4", "1/2", "1", "2", "4", "8", "16", "32", "64", "128", "256", "512"
};

static QList<QString> ScalesZ =
{
    "1/8", "1/4", "1/2", "1", "2", "4", "8", "16", "32", "64", "128", "256", "512"
};

static QList<QString> ScalesDyn =
{
    "1/8", "1/4", "1/2", "1", "2", "4", "8", "16", "32", "64", "128", "256", "512"
};

static QList<QString> ScalesMyo =
{
    "1/2", "1", "2", "4", "8", "16", "32", "64", "128", "256", "512"
};

static const double StabDefaultDiap = 128.0;
static const double ZDefaultDiap = 150.0;
static const double DynDefaultDiap = 100.0;
static const double MyoDefaultDiap = 2.0;

}


#endif // BASEUTILS_H
