#ifndef GENERALCOORDDEFINES_H
#define GENERALCOORDDEFINES_H

#include <QString>

namespace GeneralCoordDefines
{
static const QString StepDeviationTestUid = "{74B5B3BD-A29A-4F29-9404-F56568051949}";
static const QString StepOffsetTestUid = "{3664D67C-8D21-4AC8-B2DD-D5DECA43360D}";
static const QString EvolventaTestUid = "{94021282-6B50-432F-A715-D34B3FEC4FD7}";
static const QString TriangleTestUid = "{F13585D9-32C3-47CE-9C37-2B8BB7560A7D}";

/*!
 * \brief Структура, рассчитывающая и хранящая значения исходных показателей The DiapValue struct
 */
struct DiapValue
{
    QString uid;
    double min;
    double max;
    double value;
    double percent;
    DiapValue(const QString id, const double mn, const double mx) : uid(id), min(mn), max(mx) {}

    /*!
     * \brief Устанавливает значение показателя и рассчитывает процент совпадения норме
     * \param val - значение
     * \param isInverse - прямой или инверсный расчет
     */
    void setValue(const double val, const bool isInverse = true)
    {
        value = val;
        if (isInverse)
            percent = (1 - (value - min) / (max - min)) * 100;
        else
            percent = (value - min) / (max - min) * 100;
        if (percent < 0) percent = 0;
        if (percent > 100) percent = 100;
    }

    void setPercent(const double perc) {percent = perc;}
};

}


#endif // GENERALCOORDDEFINES_H
