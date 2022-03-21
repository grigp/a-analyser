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
static const QString RowsDeletedUid = "{B84238C4-77D8-4D8C-B317-49017559A999}";

static const QString TakeStageTime = "{160F146A-811D-4400-8CAE-546F7F9B6C2C}";
static const QString PutStageTime = "{7518618E-280A-471C-A75A-4358CEB32793}";
static const QString ErrorStageTime = "{CF69B4EA-3E04-47A2-A5BB-4d78E394597F}";
static const QString TakeStageSpeed = "{D607FCFA-37C7-4478-AFD1-8B6B816686DD}";
static const QString PutStageSpeed = "{41A51C05-4057-4AD1-B7B0-EC7845E6B41A}";
static const QString ErrorStageSpeed = "{9458582E-FACB-4A49-A683-72BE0BD7B91D}";
}

class Stabilogram;

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

    /*!
     * \brief Присутствуют ли показатели захвата - укладки
     */
    bool isTakePutExists() {return m_isTakePutExists;}

private:
    void getSignal();
    double getSpeed(const int begin, const int end);

    Stabilogram* m_signal {nullptr};
    int m_freqency {50};

    bool m_isTakePutExists {false};
};

#endif // TRENRESULTFACTORS_H
