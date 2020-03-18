#ifndef BREATHSIGNAL_H
#define BREATHSIGNAL_H

#include "singlesignal.h"

/*!
 * \brief Класс сигнала дыхания BreathSignal class
 */
class BreathSignal : public SingleSignal
{
public:
    explicit BreathSignal(const QString &chanId, const int freq);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit BreathSignal(const QByteArray &data);
};

#endif // BREATHSIGNAL_H
