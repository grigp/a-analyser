#ifndef TRIANGLECALCULATOR_H
#define TRIANGLECALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "baseutils.h"
#include "triangledefines.h"

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
     * \brief Возвращает индекс первого треугольника на этапе анализа
     */
    int firstAnalysisTriangle() const;

    /*!
     * \brief Длительность сигнала в пакетах
     */
    int signalLength() const;

    /*!
     * \brief Возвращает оригигальный треугольник
     */
    TriangleDefines::Triangle triangleOriginal();

    /*!
     * \brief Возвращает кол-во треугольников
     */
    int trianglesCount() const;

    /*!
     * \brief Возвращает границы треугольника по номеру
     * \param idx - номер треугольника
     */
    BaseUtils::Section triangleSection(const int idx) const;

    /*!
     * \brief Возвращает треугольник по номеру
     * \param idx - номер треугольника
     */
    TriangleDefines::Triangle triangle(const int idx) const;

private:
    TriangleFactors* m_factors {nullptr};

};

#endif // TRIANGLECALCULATOR_H
