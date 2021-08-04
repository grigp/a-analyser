#ifndef OCTAEDRONCALCULATOR_H
#define OCTAEDRONCALCULATOR_H

#include <QObject>

#include "testcalculator.h"


class OctaedronFactors;

/*!
 * \brief Класс расчета результатов тренажера "Октаэдр" OctaedronCalculator class
 */
class OctaedronCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit OctaedronCalculator(const QString &testUid, QObject *parent = nullptr);

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
     * \brief Возвращает значение успешности для одного из направлений 0 - 7
     * \param idx - индекс направления 0 - 7. 0 - вперед и далее против часовой
     */
    double getValue(const int idx) const;

    /*!
     * \brief Возвращает среднее значение успешности
     */
    double getAverageValue() const;

    int stageTime() const;
    QString circeRoundRuleMode() const;
    QString directionMode() const;
    int radius() const;
    int freq() const;
    int diap() const;

private:
    OctaedronFactors *m_factors {nullptr};
};

#endif // OCTAEDRONCALCULATOR_H
