#ifndef JUMPHEIGHTFACTORS_H
#define JUMPHEIGHTFACTORS_H

#include <QObject>
#include <QCoreApplication>

#include "multifactor.h"
#include "signalsdefines.h"

namespace JumpHeightFactorsDefines
{
static const QString GroupUid = "{5EF6AB00-42FA-458A-8CBE-82AE09A08160}";
static const QString GroupName = QCoreApplication::tr("Показатели высоты прыжков");

static const QString JumpsCountUid = "{E5E2FA90-BA27-4027-9CA6-40A5372982FF}";
static const QString FullTimeUid = "{E9133633-9E88-49A3-89C7-E8E7E13FB9D7}";
static const QString JumpsHeightAvrUid = "{9EB8FBE9-EC3A-44A4-85EB-57B93E6D39BF}";
static const QString JumpsHeightMaxUid = "{9ABC842C-06AD-4916-B648-41C5EC9DB389}";
static const QString JumpsContactTimeAvrUid = "{984D9F93-6D42-4AF7-98D0-660E56A28CE0}";
}


/*!
 * \brief Класс показателей высоты серии прыжков JumpHeightFactors class
 */
class JumpHeightFactors : public ChannelMultifactor
{
public:
    explicit JumpHeightFactors(const QString &testUid,
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
   int jumpsCount() const;

   /*!
    * \brief Возвращает данные о прыжке по его номеру
    * \param idx
    */
   SignalsDefines::JumpRec jump(const int idx) const;

private:
   double m_time {0};
   QList<SignalsDefines::JumpRec> m_jumps;
};

#endif // JUMPHEIGHTFACTORS_H
