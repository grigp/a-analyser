#ifndef STEPOFFSETCALCULATOR_H
#define STEPOFFSETCALCULATOR_H

#include <QObject>

#include "testcalculator.h"

class StepOffsetFactors;

class StepOffsetCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit StepOffsetCalculator(const QString &testUid, QObject *parent = nullptr);

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
    StepOffsetFactors *m_factors {nullptr};
};

#endif // STEPOFFSETCALCULATOR_H
