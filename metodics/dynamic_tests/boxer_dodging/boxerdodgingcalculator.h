#ifndef BOXERDODGINGCALCULATOR_H
#define BOXERDODGINGCALCULATOR_H

#include <QObject>

#include "testcalculator.h"

class BoxerDodgingMultifactor;

class BoxerDodgingCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit BoxerDodgingCalculator(const QString &testUid, QObject *parent = nullptr);


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
    BoxerDodgingMultifactor *m_factors {nullptr};
};

#endif // BOXERDODGINGCALCULATOR_H
