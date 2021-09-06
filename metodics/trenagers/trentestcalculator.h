#ifndef TRENTESTCALCULATOR_H
#define TRENTESTCALCULATOR_H

#include <QObject>

#include "testcalculator.h"

/*!
 * \brief Класс калькулятора для расчета тестов тренажеров TrenTestCalculator class
 */
class TrenTestCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit TrenTestCalculator(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    void calculate() override;

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    void fastCalculate() override;

    int factorsCount() const {return m_factors.size();}

    FactorsDefines::FactorValue factor(const int idx) const;

private:
    QList<FactorsDefines::FactorValue> m_factors;

};

#endif // TRENTESTCALCULATOR_H
