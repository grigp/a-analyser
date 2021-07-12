#ifndef EVOLVENTARESULTDATA_H
#define EVOLVENTARESULTDATA_H

#include <QObject>

#include "channeljsondata.h"
#include "factorsdefines.h"

/*!
 * \brief Класс для записи в БД данных о результатах теста на устойчивость CrossResultData class
 */
class EvolventaResultData : public ChannelJsonData
{
public:
    explicit EvolventaResultData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit EvolventaResultData(const QByteArray &data);

    int timeUpwinding() const;
    void setTimeUpwinding(const int time);

    int timeHold() const;
    void setTimeHold(const int time);

    int timeConvolution() const;
    void setTimeConvolution(const int time);

    int freq() const;
    void setFreq(const int freq);

    int diap() const;
    void setDiap(const int diap);

};


#endif // EVOLVENTARESULTDATA_H
