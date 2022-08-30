#ifndef FILTERHIGHFREQUENCY_H
#define FILTERHIGHFREQUENCY_H

#include <QObject>
#include "signaltransformer.h"

class FilterHighFrequency : public SignalTransformer
{
public:
    FilterHighFrequency();

    /*!
     * \brief Уникальный идентификатор преобразователя
     */
    QString uid() override;

    /*!
     * \brief Название преобразователя
     */
    QString name() override;

    /*!
     * \brief Преобразование
     * \param buffer - буфер сигнала. На входе - исходный сигнал, на выходе - преобразованный
     */
    void transform(QVector<double> &buffer, const QJsonObject& params) override;

    /*!
     * \brief Создает виджет редактирования параметров преобразователя и возвращает указатель на него
     */
    SignalTransformerParamsWidget* getParamsWidget() override;

};

#endif // FILTERHIGHFREQUENCY_H
