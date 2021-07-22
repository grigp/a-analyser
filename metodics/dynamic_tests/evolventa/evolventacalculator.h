#ifndef EVOLVENTACALCULATOR_H
#define EVOLVENTACALCULATOR_H

#include <QObject>

#include "testcalculator.h"
#include "evolventadefines.h"

class EvolventaFactors;
class EvolventaResultData;
class TestResultData;
class DecartCoordinatesSignal;
class ProbeResultInfo;

class EvolventaCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit EvolventaCalculator(const QString &testUid, QObject *parent = nullptr);

    ~EvolventaCalculator() override;

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
     * \brief Доступ к сигналаи, содержащим координату стабилограммы и ее целевую траекторию
     */
    DecartCoordinatesSignal *frontal() const {return m_frontal;}
    DecartCoordinatesSignal *sagittal() const {return m_sagittal;}

    int timeUpwinding() const;
    int timeHold() const;
    int timeConvolution() const;

    int diap() const;
    int freq() const;

    /*!
     * \brief Возвращает кол-во показателей эвольвенты
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

private:

    /*!
     * \brief ProbeResultInfo
     * \param pri - результаты пробы
     */
    void fillEvolventaSignal(ProbeResultInfo *pri);

    TestResultData* m_trd {nullptr};
    EvolventaFactors *m_factors {nullptr};

    DecartCoordinatesSignal *m_frontal {nullptr};
    DecartCoordinatesSignal *m_sagittal {nullptr};

    EvolventaResultData *m_erData {nullptr};
};

#endif // EVOLVENTACALCULATOR_H
