#ifndef TRIANGLECALCULATOR_H
#define TRIANGLECALCULATOR_H

#include <QObject>

#include "testcalculator.h"

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

private:
    TriangleFactors* m_factors {nullptr};

};

#endif // TRIANGLECALCULATOR_H
