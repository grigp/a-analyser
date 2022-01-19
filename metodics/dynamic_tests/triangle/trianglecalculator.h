#ifndef TRIANGLECALCULATOR_H
#define TRIANGLECALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "basedefines.h"
#include "triangledefines.h"

class TriangleFactors;
class TriangleConslutionFactors;

class TriangleCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit TriangleCalculator(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    void calculate() override;

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    void fastCalculate() override;

    int freq() const;
    int diap() const;

    /*!
     * \brief Длительность этапа обучения в пакетах
     */
    int trainingLength() const;

    /*!
     * \brief Возвращает индекс первого треугольника на этапе анализа
     */
    int firstAnalysisTriangle() const;

    /*!
     * \brief Длительность сигнала в пакетах
     */
    int signalLength() const;

    /*!
     * \brief Возвращает оригигальный треугольник
     */
    TriangleDefines::Triangle triangleOriginal();

    /*!
     * \brief Возвращает усредненный треугольник для этапа обучения
     */
    TriangleDefines::Triangle triangleTraining();

    /*!
     * \brief Возвращает усредненный треугольник для этапа анализа
     */
    TriangleDefines::Triangle triangleAnalysis();

    /*!
     * \brief Возвращает кол-во треугольников
     */
    int trianglesCount() const;

    /*!
     * \brief Возвращает границы треугольника по номеру
     * \param idx - номер треугольника
     */
    BaseDefines::Section triangleSection(const int idx) const;

    /*!
     * \brief Возвращает треугольник по номеру
     * \param idx - номер треугольника
     */
    TriangleDefines::Triangle triangle(const int idx) const;

    /*!
     * \brief Возвращает кол-во показателей
     */
    int factorCount() const;

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

    /*!
     * \brief Возвращает количество показателей, распределенных по этапам в таблице
     */
    int getFactorsOfStagesCount();

    /*!
     * \brief Возвращает uid показателей, распределенных по этапам в таблице
     * \param idx - индекс показателя
     * \param uidT - uid показателя для этапа обучения
     * \param uidA - uid показателя для этапа анализа
     * \return true, если удачно
     */
    bool getFactorOfStages(const int idx, QString &uidT, QString &uidA);



private:
    TriangleFactors* m_factors {nullptr};

};

#endif // TRIANGLECALCULATOR_H
