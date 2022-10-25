#ifndef MOTIONRECOGNITION_H
#define MOTIONRECOGNITION_H

#include <QObject>

#include "signaltransformer.h"
#include "baseutils.h"

/*!
 * \brief Класс преобразователя сигналов - выявление движений лежащего человека The MotionRecognition class
 */
class MotionRecognition : public SignalTransformer
{
public:
    MotionRecognition();

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

private:
    void computeMQ(QVector<double> &buffer, const int begin, const int end, double &m, double &q);

    struct Part
    {
        int begin {0};
        int end {0};
        Part(const int b, const int e) : begin(b), end(e) {}
    };

    QVector<double> m_sko;
    QList<Part> m_parts;

};

#endif // MOTIONRECOGNITION_H
