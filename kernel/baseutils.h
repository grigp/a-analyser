#ifndef BASEUTILS_H
#define BASEUTILS_H

#include <QObject>
#include <QPainter>

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


}


#endif // BASEUTILS_H
