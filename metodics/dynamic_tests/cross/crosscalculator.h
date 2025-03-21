#ifndef CROSSCALCULATOR_H
#define CROSSCALCULATOR_H

#include "testcalculator.h"
#include "crossdefines.h"

class CrossFactors;

/*!
 * \brief Класс расчета результатов теста на устойчивость CrossCalculator class
 */
class CrossCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit CrossCalculator(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    void calculate() override;

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    void fastCalculate() override;

    int diap() const;

    double valueUp() const;
    double valueDown() const;
    double valueRight() const;
    double valueLeft() const;

private:
    CrossFactors *m_factors {nullptr};
};

#endif // CROSSCALCULATOR_H
