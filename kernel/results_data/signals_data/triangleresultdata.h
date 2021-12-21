#ifndef TRIANGLERESULTDATA_H
#define TRIANGLERESULTDATA_H

#include <QObject>
#include <QPointF>

#include "channeljsondata.h"
#include "factorsdefines.h"

/*!
 * \brief Класс для записи в БД данных о результатах теста Треугольник TriangleResultData class
 */
class TriangleResultData : public ChannelJsonData
{
public:
    TriangleResultData(const QString &chanId);

    /*!
     * \brief Создает на основе массива байт
     * \param data - сигнал в виде массива байт
     */
    explicit TriangleResultData(const QByteArray &data);

    int freq() const;
    void setFreq(const int freq);

    int diap() const;
    void setDiap(const int diap);

    /*!
     * \brief Длительность этапа обучения в пакетах
     */
    int trainingLength() const;
    void setTrainingLength(const int length);

    /*!
     * \brief Верхняя вершина треугольника
     */
    QPointF topCorner() const;
    void setTopCorner(const QPointF point);

    /*!
     * \brief Левая нижняя вершина треугольника
     */
    QPointF leftDownCorner() const;
    void setLeftDownCorner(const QPointF point);

    /*!
     * \brief Правая нижняя вершина треугольника
     */
    QPointF rightDownCorner() const;
    void setRightDownCorner(const QPointF point);
};

#endif // TRIANGLERESULTDATA_H
