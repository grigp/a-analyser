#ifndef TEPPINGTESTCALCULATOR_H
#define TEPPINGTESTCALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "signalsdefines.h"
#include "baseutils.h"

class TeppingTestFactors;

/*!
 * \brief Класс модуля расчета данных теппинг теста TeppingTestCalculator class
 */
class TeppingTestCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit TeppingTestCalculator(const QString &testUid, QObject *parent = nullptr);

    ~TeppingTestCalculator();

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
     * \brief Возвращает кол-во прыжков
     */
    int stepsCount(BaseUtils::Side side) const;

    /*!
     * \brief Возвращает данные о прыжке по его номеру
     * \param idx
     */
    SignalsDefines::TeppingStepRec step(BaseUtils::Side side, const int idx) const;

private:
    TeppingTestFactors* m_factors {nullptr};
};

#endif // TEPPINGTESTCALCULATOR_H
