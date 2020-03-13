#ifndef DYNAMOSIGNAL_H
#define DYNAMOSIGNAL_H

#include <QByteArray>
#include "singlesignal.h"

/*!
 * \brief Класс сигнала силомеров DynamoSignal class
 */
class DynamoSignal : public SingleSignal
{
public:
    explicit DynamoSignal(const QString &chanId, const int freq);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit DynamoSignal(const QByteArray &data);
};

#endif // DYNAMOSIGNAL_H
