#ifndef STEPOFFSETCALCULATOR_H
#define STEPOFFSETCALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "signalsdefines.h"
#include "baseutils.h"

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

    int stageTime() const;
    int freq() const;
    int diap() const;
    int force() const;
    BaseUtils::Directions direction() const;
    int stepsCount() const;

    int bufferCompensationCount() const;
    double bufferCompensationValue(const int i) const;
    int bufferReturnCount() const;
    double bufferReturnValue(const int i) const;

private:
    StepOffsetFactors *m_factors {nullptr};
};

#endif // STEPOFFSETCALCULATOR_H
