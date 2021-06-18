#include "stepoffsetcalculator.h"

StepOffsetCalculator::StepOffsetCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void StepOffsetCalculator::calculate()
{
    TestCalculator::calculate();
}

void StepOffsetCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();

}
