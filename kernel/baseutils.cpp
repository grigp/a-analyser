#include "baseutils.h"


QString BaseUtils::getTimeBySecCount(const int secCnt, const bool isHour)
{
    int min = secCnt / 60;
    int sec = secCnt % 60;
    if (isHour)
    {
        int hour = min / 60;
        min = min % 60;
        return QString::number(hour).rightJustified(2, '0') + ":" +
               QString::number(min).rightJustified(2, '0') + ":" +
               QString::number(sec).rightJustified(2, '0');
    }
    return QString::number(min).rightJustified(2, '0') + ":" +
           QString::number(sec).rightJustified(2, '0');
}
