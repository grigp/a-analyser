#ifndef BALLISTOGRAM_H
#define BALLISTOGRAM_H

#include "singlesignal.h"

/*!
 * \brief Класс сигнала баллистограммы Ballistogram class
 */
class Ballistogram : public SingleSignal
{
public:
    explicit Ballistogram(const QString &chanId, const int freq);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit Ballistogram(const QByteArray &data);

};

#endif // BALLISTOGRAM_H


