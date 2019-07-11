#ifndef STABSIGNALSTESTCALCULATOR_H
#define STABSIGNALSTESTCALCULATOR_H

#include <QObject>
#include "testcalculator.h"

class StabSignalsTestCalculator : public StabTestCalculator
{
    Q_OBJECT
public:
    StabSignalsTestCalculator(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    void calculate() override;

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    void fastCalculate() override;

private:
    void fillSignals(QModelIndex parent = QModelIndex());
    QString m_probeUid {""};
};

#endif // STABSIGNALSTESTCALCULATOR_H
