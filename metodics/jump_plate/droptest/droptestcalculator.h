#ifndef DROPTESTCALCULATOR_H
#define DROPTESTCALCULATOR_H

#include <QObject>
#include "testcalculator.h"
#include "signalsdefines.h"

class DropTestFactors;

/*!
 * \brief Класс модуля расчета данных дроп теста DropTestCalculator class
 */
class DropTestCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit DropTestCalculator(const QString &testUid, QObject *parent = nullptr);

    ~DropTestCalculator();

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
    int jumpsCount() const;

    /*!
     * \brief Возвращает данные о прыжке по его номеру
     * \param idx
     */
    SignalsDefines::DropJumpFactors jump(const int idx) const;

private:
    DropTestFactors* m_factors {nullptr};
};

#endif // DROPTESTCALCULATOR_H
