#ifndef TRENTAKEPUTTESTCALCULATOR_H
#define TRENTAKEPUTTESTCALCULATOR_H

#include <QObject>

#include "testcalculator.h"

/*!
 * \brief Класс расчета результатов тренажеров захвата - укладки TrenTakePutTestCalculator class
 */
class TrenTakePutTestCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit TrenTakePutTestCalculator(const QString &testUid, QObject *parent = nullptr);

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


#endif // TRENTAKEPUTTESTCALCULATOR_H
