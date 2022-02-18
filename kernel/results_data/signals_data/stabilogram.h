#ifndef STABILOGRAM_H
#define STABILOGRAM_H

#include "decartcoordinatessignal.h"

/*!
 * \brief Класс сигнала стабилограммы Stabilogram class
 */
class Stabilogram : public DecartCoordinatesSignal
{
public:
    explicit Stabilogram(const QString &chanId, const int freq);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit Stabilogram(const QByteArray &data);

    ~Stabilogram() override;
};


#endif // STABILOGRAM_H
