#ifndef SIGNALTRANSFORMER_H
#define SIGNALTRANSFORMER_H

#include <QObject>
#include <QJsonObject>

class SignalTransformerParamsWidget;

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
     * \param params - параметры преобразования
     */
    virtual void transform(QVector<double> &buffer, const QJsonObject& params) = 0;

    /*!
     * \brief Создает виджет редактирования параметров преобразователя и возвращает указатель на него
     */
    virtual SignalTransformerParamsWidget* getParamsWidget() = 0;
};

#endif // SIGNALTRANSFORMER_H
