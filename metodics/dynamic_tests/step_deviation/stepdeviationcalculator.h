#ifndef STEPDEVIATIONCALCULATOR_H
#define STEPDEVIATIONCALCULATOR_H

#include "testcalculator.h"
#include "basedefines.h"

#include <QObject>

class StepDeviationFactors;

/*!
 * \brief Класс расчета результатов теста прироста (теста со ступенчатым отклонением) StepDeviationCalculator class
 */
class StepDeviationCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit StepDeviationCalculator(const QString &testUid, QObject *parent = nullptr);

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
     * \brief Возвращает размер сигнала
     */
    int signalSize() const;

    /*!
     * \brief Возвращает частоту дискретизации сигнала
     */
    int signalFrequency() const;

    /*!
     * \brief Возвращает значение сигнала по индексу
     * \param idx - индекс
     */
    double signal(const int idx) const;

    /*!
     * \brief Возвращает значение фильтрованного сигнала по индексу
     * \param idx - индекс
     */
    double signalFiltred(const int idx) const;

    int growthDynCount() const;
    double growthDynValue(const int idx) const;

    int lengthDynCount() const;
    double lengthDynValue(const int idx) const;

    /*!
     * \brief Возвращает направление отклонений
     */
    BaseDefines::Directions direction() const;

private:
    StepDeviationFactors* m_factors {nullptr};

};

#endif // STEPDEVIATIONCALCULATOR_H
