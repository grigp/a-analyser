#ifndef TARGETCALCULATOR_H
#define TARGETCALCULATOR_H

#include <QObject>

#include "testcalculator.h"

class TargetFactors;

/*!
 * \brief Класс расчета результатов стабилографического теста как теста Мишень TargetCalculator class
 */
class TargetCalculator : public StabTestCalculator
{
    Q_OBJECT
public:
    explicit TargetCalculator(const QString &testUid, QObject *parent = nullptr);

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
     */
    TargetFactors* factors() const;

private:
    TargetFactors* m_factors;
};

#endif // TARGETCALCULATOR_H
