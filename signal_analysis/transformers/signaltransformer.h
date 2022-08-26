#ifndef SIGNALTRANSFORMER_H
#define SIGNALTRANSFORMER_H

#include <QObject>

/*!
 * \brief Базовый класс преобразователя сигнала The SignalTransformer class
 */
class SignalTransformer
{
public:
    SignalTransformer();
    virtual ~SignalTransformer();

    /*!
     * \brief Уникальный идентификатор преобразователя
     */
    virtual QString uid() = 0;

    /*!
     * \brief Название преобразователя
     */
    virtual QString name() = 0;

    /*!
     * \brief Преобразование
     * \param buffer - буфер сигнала. На входе - исходный сигнал, на выходе - преобразованный
     */
    virtual void transform(QVector<double> &buffer) = 0;
};

#endif // SIGNALTRANSFORMER_H
