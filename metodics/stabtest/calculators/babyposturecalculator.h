#ifndef BABYPOSTURECALCULATOR_H
#define BABYPOSTURECALCULATOR_H

#include <QObject>
#include "testcalculator.h"


class ClassicFactors;
class SpectrSingleSignalFactors;
class VectorFactors;

/*!
 * \brief Класс расчета результатов стабилографического теста как постурального контроля детей первого года жизни The BabyPostureCalculator class
 */
class BabyPostureCalculator : public StabTestCalculator
{
    Q_OBJECT
public:
    BabyPostureCalculator(const QString &testUid, QObject *parent = nullptr);
    ~BabyPostureCalculator() override;

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    void calculate() override;

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    void fastCalculate() override;

    QString ampl1() {return m_ampl1;}
    QString ampl2() {return m_ampl2;}
    QString ampl3() {return m_ampl3;}
    QString freq1() {return m_freq1;}
    QString freq2() {return m_freq2;}
    QString freq3() {return m_freq3;}

    QString square() {return m_s;}
    QString ellLen() {return m_ellLen;}
    QString lfs() {return m_lfs;}
    QString ellE() {return m_ellE;}
    QString kfr() {return m_kfr;}


private:
    ClassicFactors* m_fctClassic {nullptr};
    SpectrSingleSignalFactors* m_fctSpectr {nullptr};
    VectorFactors* m_fctVector {nullptr};

    QString m_ampl1 {""};
    QString m_ampl2 {""};
    QString m_ampl3 {""};
    QString m_freq1 {""};
    QString m_freq2 {""};
    QString m_freq3 {""};

    QString m_s {""};
    QString m_ellLen {""};
    QString m_lfs {""};
    QString m_ellE {""};
    QString m_kfr {""};
};

#endif // BABYPOSTURECALCULATOR_H
