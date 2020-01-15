#ifndef JUMPTESTCALCULATOR_H
#define JUMPTESTCALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "jumptestdefines.h"

/*!
 * \brief Класс расчета результатов прыжкового теста JumpTestCalculator class
 */
class JumpTestCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit JumpTestCalculator(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    void calculate() override;

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    void fastCalculate() override;

    JumpTestDefines::Methodic metodic() {return m_methodic;}

private:
    JumpTestDefines::Methodic m_methodic {JumpTestDefines::MetJumpHeight};
};

#endif // JUMPTESTCALCULATOR_H
