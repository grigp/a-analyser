#ifndef WEIGHINGRESULTDATA_H
#define WEIGHINGRESULTDATA_H

#include <QObject>
#include <QTime>

#include "channeljsondata.h"

class WeighingResultData : public ChannelJsonData
{
public:
    WeighingResultData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit WeighingResultData(const QByteArray &data);


    QTime scalingInterval() const;
    void setScalingInterval(const QTime interval);

    int averageTime() const;
    void setAverageTime(const int time);

    /*!
     * \brief Возвращает кол-во измерений веса в канале
     */
    int size() const;

    /*!
     * \brief Возвращает данные об измерении веса по номеру измерения
     */
    double weight(const int idx) const;

    /*!
     * \brief Возвращает данные о моменте измерения веса по номеру измерения
     */
    QDateTime dateTime(const int idx) const;

    /*!
     * \brief Добавляет данные об измерении веса в канал (в пробах)
     * \param value - значение веса
     * \param dt - дата и время измерения
     */
    void addWeight(const double value, QDateTime dt);
};

#endif // WEIGHINGRESULTDATA_H
