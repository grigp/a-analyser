#include "mwcompetitivestickcalculator.h"

MWCompetitiveStickCalculator::MWCompetitiveStickCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
{

}

void MWCompetitiveStickCalculator::calculate()
{
    TestCalculator::calculate();
}

void MWCompetitiveStickCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}
