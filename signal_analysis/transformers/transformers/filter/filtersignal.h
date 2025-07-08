#ifndef FILTERSIGNAL_H
#define FILTERSIGNAL_H

#include <QObject>

#include "signaltransformer.h"
#include "baseutils.h"

/*!
 * \brief Класс преобразователя сигналов - фильтрация The FilterSignal class
 */
class FilterSignal : public SignalTransformer
{
public:
    FilterSignal();

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

    /*!
     * \brief Возвращает QMap имен фильтров по типу локализованное
     */
    static QMap<BaseUtils::FilterKind, QString> filterName();

    /*!
     * \brief Возвращает QMap имен типов фильтров локализованное
     */
    static QMap<BaseUtils::FilterDirection, QString> filterDirectionName();


private:
    void filtraton(QVector<double> &buffer, BaseUtils::FilterDirection dir, double freqCutoff, BaseUtils::FilterKind kind, int freqSample);

};

#endif // FILTERSIGNAL_H
