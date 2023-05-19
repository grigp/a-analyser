#ifndef RITMOGRAM_H
#define RITMOGRAM_H

#include "singlesignal.h"

/*!
 * \brief Класс сигнала ритмограммы Ritmogram class
 */
class Ritmogram : public SingleSignal
{
public:
    explicit Ritmogram(const QString &chanId, const int freq);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit Ritmogram(const QByteArray &data);

};

#endif // RITMOGRAM_H
