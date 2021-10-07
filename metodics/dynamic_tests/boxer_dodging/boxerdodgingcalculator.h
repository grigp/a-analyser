#ifndef BOXERDODGINGCALCULATOR_H
#define BOXERDODGINGCALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "boxerdodgingdefines.h"

class BoxerDodgingMultifactor;

class BoxerDodgingCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit BoxerDodgingCalculator(const QString &testUid, QObject *parent = nullptr);


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
     * \brief Возвращает значение показателя по его uid
     */
    double factorValue(const QString uid) const;

    /*!
     * \brief Возвращает форматированное значение показателя в строковом виде по uid показателя
     */
    QString factorValueFormatted(const QString &uid) const;

    /*!
     * \brief Доступ к отдельным данным расчета
     */
    int deviationThreshold() const;
    int freq() const;
    int diap() const;

    int tryesCount() const;
    int leftCount() const;
    int rightCount() const;
    int aheadCount() const;
    int backCount() const;
    int count(const BoxerDodgingDefines::Stages stage) const;

private:
    BoxerDodgingMultifactor *m_factors {nullptr};
};

#endif // BOXERDODGINGCALCULATOR_H
