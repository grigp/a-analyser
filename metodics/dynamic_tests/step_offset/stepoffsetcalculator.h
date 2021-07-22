#ifndef STEPOFFSETCALCULATOR_H
#define STEPOFFSETCALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "signalsdefines.h"
#include "baseutils.h"

class StepOffsetFactors;

class StepOffsetCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit StepOffsetCalculator(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    void calculate() override;

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    void fastCalculate() override;

    int stageTime() const;
    int freq() const;
    int diap() const;
    int force() const;
    BaseUtils::Directions direction() const;
    int stepsCount() const;

    int bufferCompensationCount() const;
    double bufferCompensationValue(const int i) const;
    int bufferReturnCount() const;
    double bufferReturnValue(const int i) const;

    /*!
     * \brief Возвращает кол-во показателей в группе
     */
    int factorsCount() const;

    /*!
     * \brief Возвращает uid показателя по номеру в списке
     * \param id - номер показателя в списке
     */
    QString factorUid(const int id) const;

    /*!
     * \brief Возвращает значение показателя по номеру в списке
     * \param id - номер показателя в списке
     */
    double factorValue(const int id) const;

    /*!
     * \brief Возвращает значение показателя по uid показателя
     * \param uid - uid показателя
     */
    double factorValue(const QString &uid) const;

    /*!
     * \brief Возвращает форматированное значение показателя в строковом виде по uid показателя
     * \param uid - uid показателя
     */
    QString factorValueFormatted(const QString &uid) const;


private:
    StepOffsetFactors *m_factors {nullptr};
};

#endif // STEPOFFSETCALCULATOR_H
