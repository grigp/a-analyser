#ifndef BASEUTILS_H
#define BASEUTILS_H

#include <QObject>

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

}


#endif // BASEUTILS_H
