#ifndef JUMPHEIGHTTESTCALCULATOR_H
#define JUMPHEIGHTTESTCALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "signalsdefines.h"

class JumpHeightFactors;

class JumpHeightTestCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit JumpHeightTestCalculator(const QString &testUid, QObject *parent = nullptr);

    ~JumpHeightTestCalculator();

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
    SignalsDefines::JumpRec jump(const int idx) const;

private:
    JumpHeightFactors* m_factors {nullptr};
};

#endif // JUMPHEIGHTTESTCALCULATOR_H
