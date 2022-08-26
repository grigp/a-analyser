#ifndef FILTERLOWFREQUENCY_H
#define FILTERLOWFREQUENCY_H

#include <QObject>
#include "signaltransformer.h"

class FilterLowFrequency : public SignalTransformer
{
public:
    FilterLowFrequency();

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
    void transform(QVector<double> &buffer) override;
};

#endif // FILTERLOWFREQUENCY_H
