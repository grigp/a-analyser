#ifndef PULSEFACTORS_H
#define PULSEFACTORS_H

#include <QObject>
#include <QCoreApplication>
#include "multifactor.h"


namespace PulseFactorsDefines
{
static const QString GroupUid = "{BAED5445-97FE-48B9-99DF-1A5FF367A870}";
static const QString GroupName = QCoreApplication::tr("Показатели вариационной пульсометрии");

static const QString PulseUid = "{6930C0E6-6858-4BE3-8040-6A9E130FA0E1}";
}

/*!
 * \brief Класс модуля расчета показателей ритмограммы The PulseFactors class
 */
class PulseFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    PulseFactors(const QString &testUid,
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

private:
    double m_pulse {0};
};

#endif // PULSEFACTORS_H
