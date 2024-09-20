#ifndef BABYPOSTURECALCULATOR_H
#define BABYPOSTURECALCULATOR_H

#include <QObject>
#include "testcalculator.h"
#include "datadefines.h"


class ClassicFactors;
class SpectrStabFactors;
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
     * \brief Структура данных о показателе The FactorInfo struct
     */
    struct FactorInfo
    {
        QString uid {""};       ///< uid
        int group {-1};         ///< код группы показателей 0-classic, 1-spectrStab, 2-spectrZ, 3-vector
        QString name {""};      ///< Название
        QString shortName {""}; ///< Обозначение
        QString valueFmt {""};  ///< Форматированное значение
        FactorInfo() {}
        FactorInfo(std::tuple<QString, int, QString, QString, QString> params)
            : uid(std::get<0>(params))
            , group(std::get<1>(params))
            , name(std::get<2>(params))
            , shortName(std::get<3>(params))
            , valueFmt(std::get<4>(params))
        {}
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

    int factorsCount() const;
    FactorInfo factor(const int idx) const;

    int factors1Count() const;
    FactorInfo factor1(const int idx) const;

    int factors2Count() const;
    FactorInfo factor2(const int idx) const;

private:
    QList<FactorInfo> createFactors();

    void assignFactors(const QString& ciStab, const QString& ciZ,
                       DataDefines::ProbeInfo& pi,
                       QList<BabyPostureCalculator::FactorInfo>& factors,
                       const bool isPrimary);

    ClassicFactors* m_fctClassic {nullptr};
    SpectrStabFactors* m_fctSpectrStab {nullptr};
    SpectrSingleSignalFactors* m_fctSpectrZ {nullptr};
    VectorFactors* m_fctVector {nullptr};

    QList<FactorInfo> m_factors;
    QList<FactorInfo> m_factors1;
    QList<FactorInfo> m_factors2;
};

#endif // BABYPOSTURECALCULATOR_H
