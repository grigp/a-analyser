#ifndef BEDSIDESCALESTESTCALCULATOR_H
#define BEDSIDESCALESTESTCALCULATOR_H

#include <QObject>

#include "testcalculator.h"

class SignalAccess;

/*!
 * \brief Класс рассчета данных для прикроватных весов The BedsideScalesTestCalculator class
 */
class BedsideScalesTestCalculator : public TestCalculator
{
    Q_OBJECT
public:
    explicit BedsideScalesTestCalculator(const QString &testUid, QObject *parent = nullptr);
    ~BedsideScalesTestCalculator() override;

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
     * \brief Возвращает частоту дискретизации канала
     */
    int signalFrequency() const;

    /*!
     * \brief Возвращает размер сигнала в отсчетах
     */
    int signalSize() const;

    /*!
     * \brief Возвращает кол-во подканалов
     */
    int subChansCount() const;

    /*!
     * \brief Возвращает данные канала
     * \param subChan - номер подканала (отведения)
     * \param rec - номер записи
     * \return значение сигнала п заданным номеру записи и номеру подканала
     */
    double signalValue(const int subChan, const int rec) const;


private:
    void getSignal(const QString &probeId);

    SignalAccess* m_signal {nullptr};
};

#endif // BEDSIDESCALESTESTCALCULATOR_H