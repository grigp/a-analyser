#ifndef IDSFACTORS_H
#define IDSFACTORS_H

#include <QObject>
#include <QVector>
#include <QCoreApplication>

#include "multifactor.h"

namespace IDSFactorsDefines
{
static const QString GroupUid = "{603029E3-72A2-4050-A7F2-B65DFC0E63EB}";
static const QString GroupName = QCoreApplication::tr("Показатели динамической стабилизации");

static const QString IDSUid = "{E7C3FC19-2342-4969-8E29-C9BE88646816}";
static const QString FDSDispUid = "{57E26D17-C246-4AB9-8A89-27F60E8DE624}";
static const QString FreqUid = "{AF9F0787-F741-4FDD-A245-362DB4B729F4}";
static const QString StabFDSUid = "{692B3ABA-F7A3-4E91-A23E-D05B2C788290}";
static const QString AsymFDSUid = "{04D884ED-5704-4B88-B8F8-E183E1F4B7A0}";
static const QString KVarFDSUid = "{CBA60DED-B282-47A5-B29E-2170364C9763}";
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
               const int begin = -1,
               const int end  = -1,
               QObject *parent = nullptr);

    /*!
     * \brief Возвращает uid группы показателей
     */
    QString uid() const override;

    /*!
     * \brief Возвращает название группы показателей
     */
    QString name() const override;

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

    /*!
     * \brief Возвращает чатоту дискретизации сигнала
     */
    int freqDiskr() {return m_freqDiskr;}

private:
    void computeSpeedBuf(Stabilogram* stab, QVector<double> &bufV, QVector<double> &bufW);

    void computeFDSBuf(const QVector<double> &bufV, const QVector<double> &bufW);

    void computeStabilityFDS();

    void computeAsymmetryFDS();

    void bufToFile(const QVector<double> &buf, const QString &fn);

    int m_freqDiskr {0};        ///< Частота дискретизации сигнала
    QVector<double> m_bufFDS;   ///< Массив значений ФДС
    double m_fdsQ {0};          ///< СКО ФДС

    double m_ids {0};      ///< ИДС
    double m_freq {1};     ///< Частота звука
    double m_stabFDS {0};  ///< Стабильность ФДС
    double m_asymFDS {0};  ///< Асимметрия ФДС
    double m_kvFDS {0};    ///< Коэф-т вариации ФДС
};

#endif // IDSFACTORS_H
