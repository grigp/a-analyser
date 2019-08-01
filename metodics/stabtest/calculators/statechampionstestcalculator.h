#ifndef STATECHAMPIONSTESTCALCULATOR_H
#define STATECHAMPIONSTESTCALCULATOR_H

#include <QObject>
#include "testcalculator.h"

/*!
 * \brief Класс расчета результатов стабилографического теста как тестирование "Стань чемпионом" StateChampionsTestCalculator class
 */
class StateChampionsTestCalculator : public StabTestCalculator
{
public:
    StateChampionsTestCalculator(const QString &testUid, QObject *parent = nullptr);

    ///< Показатели по пробам
    struct ProbesFactors
    {
        double speed;
        double square;
    };

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
     * \brief Возвращает значение скорости для пробы по номеру
     */
    double speed(const int probeNum) const;

    /*!
     * \brief Возвращает значение площади эллипса для пробы по номеру
     */
    double square(const int probeNum) const;

    /*!
     * \brief Возвращает значение коэффициента Ромберга
     */
    double rombKoef() const;

    /*!
     * \brief Возвращает значение КФР для пробы "Мишень"
     */
    double targetKFR() const;

private:
    ProbesFactors m_prbFct[3];
    double m_RombKoef;
    double m_trgtKFR;
};

#endif // STATECHAMPIONSTESTCALCULATOR_H
