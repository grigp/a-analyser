#ifndef BASEUTILS_H
#define BASEUTILS_H

#include <QObject>
#include <QPainter>
#include <QAbstractItemModel>

namespace BaseUtils
{

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

static QMap<Directions, QString> DirectionValueName {
    std::pair<Directions, QString> (dirUp, "up")
  , std::pair<Directions, QString> (dirRight, "right")
  , std::pair<Directions, QString> (dirDown, "down")
  , std::pair<Directions, QString> (dirLeft, "left")
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


}


#endif // BASEUTILS_H
