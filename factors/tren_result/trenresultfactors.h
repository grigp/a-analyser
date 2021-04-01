#ifndef TRENRESULTFACTORS_H
#define TRENRESULTFACTORS_H

#include <QObject>

#include "multifactor.h"

namespace TrenResultFactorsDefines
{
static const QString GroupUid = "{CAEBCA5C-7F3F-403C-91BB-F7D7016C5E5F}";
static const QString ScoreUid = "{ED2E454B-DF22-462C-BE44-B9DCFD1A284B}";
static const QString FaultsUid = "{B492963B-F280-4E23-A97A-88E1172B53A0}";
static const QString TimeUid = "{B24D1602-8905-4616-B121-DBB9661D5E40}";
}


/*!
 * \brief Класс модуля расчета показателей результатов сеанса тренинга TrenResultFactors class
 */
class TrenResultFactors : public ChannelMultifactor
{
public:
    explicit TrenResultFactors(const QString &testUid,
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
};

#endif // TRENRESULTFACTORS_H
