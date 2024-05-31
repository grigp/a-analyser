#ifndef MWCOMPETITIVESTICKCALCULATOR_H
#define MWCOMPETITIVESTICKCALCULATOR_H

#include <QObject>

#include "testcalculator.h"

class MWStickForceFactors;

/*!
 * \brief Класс расчета результатов теста с соревновательной палкой The MWCompetitiveStickCalculator class
 */
class MWCompetitiveStickCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit MWCompetitiveStickCalculator(const QString &testUid, QObject *parent = nullptr);

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
     * \brief Возвращает максимальное усилие
     */
    double maxForce();
    QString maxForceFmt();

    /*!
     * \brief Возвращает среднее усилие
     */
    double averageForce();
    QString averageForceFmt();

private:
    MWStickForceFactors* m_factors {nullptr};

};

#endif // MWCOMPETITIVESTICKCALCULATOR_H
