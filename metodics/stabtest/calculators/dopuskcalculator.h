#ifndef DOPUSKCALCULATOR_H
#define DOPUSKCALCULATOR_H

#include <QObject>
#include "testcalculator.h"

class VectorFactors;

class DopuskCalculator : public StabTestCalculator
{
    Q_OBJECT
public:
    DopuskCalculator(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    void calculate() override;

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    void fastCalculate() override;

    /*!
     * \brief Возвращает указатель на факторгруппу, рассчитанную для пробы по номеру
     * \param probeNum - номер пробы
     */
    VectorFactors* factors(const int probeNum) const;

private:
    VectorFactors* m_factors[3];
};

#endif // DOPUSKCALCULATOR_H
