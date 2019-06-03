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

}


#endif // BASEUTILS_H
