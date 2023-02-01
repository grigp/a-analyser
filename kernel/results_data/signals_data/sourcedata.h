#ifndef SOURCEDATA_H
#define SOURCEDATA_H

#include <QObject>

#include "myogram.h"

/*!
 * \brief Класс для записи в БД данных исходных сигналов The SourceData class
 * subChansCount() каналов типа double
 */
class SourceData : public Myogram
{
public:
    explicit SourceData(const QString &chanId, const int subChansCount, const int freq);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit SourceData(const QByteArray &data);
};

#endif // SOURCEDATA_H
