#ifndef IDSFACTORS_H
#define IDSFACTORS_H

#include <QObject>
#include <QVector>

#include "multifactor.h"

namespace IDSFactorsDefines
{
static const QString GroupUid = "{603029E3-72A2-4050-A7F2-B65DFC0E63EB}";
static const QString IDSUid = "{E7C3FC19-2342-4969-8E29-C9BE88646816}";
static const QString FDSDispUid = "{57E26D17-C246-4AB9-8A89-27F60E8DE624}";
static const QString FreqUid = "{AF9F0787-F741-4FDD-A245-362DB4B729F4}";
}

class Stabilogram;

/*!
 * \brief Класс мультифактора для расчета показателей ИДС IDSFactors class
 */
class IDSFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    IDSFactors(const QString &testUid,
               const QString &probeUid,
               const QString &channelId,
               QObject *parent = nullptr);

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * uid теста задается в конструкторе
     */
    bool isValid()  const override;
    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * Аналог для вызова без создания экземпляра объекта
     */
    static bool isValid(const QString &testUid,
                        const QString &probeUid,
                        const QString &channelId);

    /*!
     * \brief Рассчитывает показатели для теста с заданным uid
     * uid теста задается в конструкторе
     */
    void calculate() override;

    /*!
     * \brief Регистрирует факторсгруппу и показатели
     */
    static void registerFactors();

    /*!
     * \brief Возвращает размер буфера ФДС
     */
    int fdsSize() const {return m_bufFDS.size();}

    /*!
     * \brief Возвращает значение ФДС по индексу
     * \param idx - индекс записи
     */
    double fds(const int idx) const;

private:
    void computeSpeedBuf(Stabilogram* stab, QVector<double> &bufV, QVector<double> &bufW);

    void computeFDSBuf(const QVector<double> &bufV, const QVector<double> &bufW);

    QVector<double> m_bufFDS;   ///< Массив значений ФДС
    double m_fdsQ {0};          ///< СКО ФДС

    double m_ids {0};    ///< ИДС
    double m_freq {1};   ///< Частота звука
};

#endif // IDSFACTORS_H
