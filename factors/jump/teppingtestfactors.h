#ifndef TEPPINGTESTFACTORS_H
#define TEPPINGTESTFACTORS_H

#include <QObject>

#include "multifactor.h"
#include "signalsdefines.h"
#include "baseutils.h"

namespace TeppingTestFactorsDefines
{
static const QString GroupUid = "{992249B4-C024-4FD6-A2CD-EF65764A927F}";
static const QString StepsCountUid = "{590755BB-001A-4566-B780-48657CEBDC5F}";
static const QString FullTimeUid = "{C6B9224A-2458-4D2E-8058-C0DC2E718736}";
static const QString LeftLegContactTimeAvrUid = "{C2642BE7-531A-49EC-S2AB-4CB76C8A777E}";
static const QString RightLegContactTimeAvrUid = "{13D80F37-198C-4BCD-84DA-816A24C16E44}";
}

/*!
 * \brief Класс показателей теппинг - теста TeppingTestFactors class
 */
class TeppingTestFactors : public ChannelMultifactor
{
public:
    explicit TeppingTestFactors(const QString &testUid,
                                const QString &probeUid,
                                const QString &channelId,
                                QObject *parent = nullptr);

    /*! \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
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
    * \brief Возвращает кол-во прыжков
    */
   int stepsCount(BaseUtils::Side side) const;

   /*!
    * \brief Возвращает данные о прыжке по его номеру
    * \param idx
    */
   SignalsDefines::TeppingStepRec step(BaseUtils::Side side, const int idx) const;

private:
   double m_time {0};
   QList<SignalsDefines::TeppingStepRec> m_stepsLeftLeg, m_stepsRightLeg;
};

#endif // TEPPINGTESTFACTORS_H