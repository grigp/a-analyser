#ifndef TRIANGLECALCULATOR_H
#define TRIANGLECALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "baseutils.h"

class TriangleFactors;

class TriangleCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit TriangleCalculator(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    void calculate() override;

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    void fastCalculate() override;

    int freq() const;
    int diap() const;

    /*!
     * \brief Длительность этапа обучения в пакетах
     */
    int trainingLength() const;

    /*!
     * \brief Длительность сигнала в пакетах
     */
    int signalLength() const;

    /*!
     * \brief Верхняя вершина треугольника
     */
    QPointF topCorner() const;

    /*!
     * \brief Левая нижняя вершина треугольника
     */
    QPointF leftDownCorner() const;

    /*!
     * \brief Правая нижняя вершина треугольника
     */
    QPointF rightDownCorner() const;

    /*!
     * \brief Возвращает кол-во треугольников
     */
    int trianglesCount() const;

    /*!
     * \brief Возвращает границы треугольника по номеру
     * \param idx - номер треугольника
     */
    BaseUtils::Section triangle(const int idx) const;

private:
    TriangleFactors* m_factors {nullptr};

};

#endif // TRIANGLECALCULATOR_H
