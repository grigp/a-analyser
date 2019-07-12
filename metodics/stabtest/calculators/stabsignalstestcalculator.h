#ifndef STABSIGNALSTESTCALCULATOR_H
#define STABSIGNALSTESTCALCULATOR_H

#include <QObject>
#include "testcalculator.h"

class ClassicFactors;

struct StabProbeFactors
{
    ClassicFactors *fctClassic;
    StabProbeFactors() {}
};

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

    /*!
     * \brief Возвращает кол-во проб в тесте
     */
    int probesCount() const {return m_probesFactors.size();}

    /*!
     * \brief Возвращает кол-во показателей
     */
    int factorsCount() const;

    /*!
     * \brief Возвращает классические показатели для пробы по номеру
     * \param probeNum - номер пробы
     */
    ClassicFactors *classicFactors(const int probeNum) const;

private:
    //void fillSignals(QModelIndex parent = QModelIndex());

    QString m_probeUid {""};
    QList<StabProbeFactors> m_probesFactors;
};

#endif // STABSIGNALSTESTCALCULATOR_H
