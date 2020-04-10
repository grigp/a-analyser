#ifndef STRESSSTRATEGYCALCULATOR_H
#define STRESSSTRATEGYCALCULATOR_H

#include <QObject>

#include "testcalculator.h"

class VectorFactors;

/*!
 * \brief Класс расчета результатов стабилографического теста как теста оценки стрессовой стратегии StressStrategyCalculator class
 */
class StressStrategyCalculator : public StabTestCalculator
{
    Q_OBJECT
public:
    StressStrategyCalculator(const QString &testUid, QObject *parent = nullptr);

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
     * \brief Возвращает кол-во проб
     */
    int probesCount() const;

    /*!
     * \brief Возвращает указатель на факторгруппу, рассчитанную для пробы по номеру
     */
    VectorFactors* factors(const int probeNum) const;

    double fctTarget() const {return m_fctTarget;}
    double fctZones() const {return m_fctZones;}

private:
    QList<VectorFactors*> m_factors;
    double m_fctTarget {0};
    double m_fctZones {0};
};

#endif // STRESSSTRATEGYCALCULATOR_H
