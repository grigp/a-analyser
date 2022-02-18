#ifndef BALISTOGRAM_H
#define BALISTOGRAM_H

#include "singlesignal.h"

/*!
 * \brief Класс сигнала баллистограммы Ballistogram class
 */
class Balistogram : public SingleSignal
{
public:
    explicit Balistogram(const QString &chanId, const int freq);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit Balistogram(const QByteArray &data);

};

#endif // BALISTOGRAM_H


