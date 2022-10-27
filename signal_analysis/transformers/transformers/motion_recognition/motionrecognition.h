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

    struct Part
    {
        int begin {0};
        int end {0};
        Part(const int b, const int e) : begin(b), end(e) {}
    };

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
     * \brief Задает значения для отсутствия и наличия движений
     * \param noMotion - нет движений
     * \param presentMotion - есть движения
     */
    void setValues(const int noMotion, const int presentMotion);

    /*!
     * \brief Возвращает кол-во участков без двигательной активности
     */
    int partsNoMotionCount() const;

    /*!
     * \brief Возвращает параметра участка без двигательной активности по номеру
     * \param idx - номер участка
     */
    Part partNoMotion(const int idx) const;

private:
    void computeMQ(QVector<double> &buffer, const int begin, const int end, double &m, double &q);

    QVector<double> m_sko;
    QList<Part> m_partsMotions;    ///< Участки активных движений
    QList<Part> m_partsNoMotions;  ///< Участки без активных движений

    int m_noMotion {0};
    int m_presentMotion {1};
};

#endif // MOTIONRECOGNITION_H
