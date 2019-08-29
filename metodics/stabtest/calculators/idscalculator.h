#ifndef IDSCALCULATOR_H
#define IDSCALCULATOR_H

#include <QObject>
#include <QList>

#include "testcalculator.h"

class IDSFactors;

/*!
 * \brief Класс расчета результатов стабилографического теста как анализа уровня здоровья по ИДС IDSCalculator class
 */
class IDSCalculator : public StabTestCalculator
{
    Q_OBJECT
public:
    IDSCalculator(const QString &testUid, QObject *parent = nullptr);

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
    IDSFactors* factors(const int probeNum) const;

private:
    QList<IDSFactors*> m_factors;
};

#endif // IDSCALCULATOR_H
