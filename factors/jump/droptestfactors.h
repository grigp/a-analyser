#ifndef DROPTESTFACTORS_H
#define DROPTESTFACTORS_H

#include <QObject>
#include <QCoreApplication>

#include "multifactor.h"
#include "signalsdefines.h"

namespace DropTestFactorsDefines
{
static const QString GroupUid = "{59D5CDE6-E52B-4143-B542-36998DABC57F}";

static const QString JumpsCountUid = "{6864BC37-310E-4240-AA1D-1923629268AB}";
static const QString FullTimeUid = "{256972A2-9CCF-4779-A898-40B85D461E65}";
static const QString JumpsHeightAvrUid = "{6D3A84EB-F8B3-4F9C-A2DF-A9D9D8568F0D}";
static const QString JumpsContactTimeAvrUid = "{39D37FBA-79F0-4EF5-A729-9F3198233C91}";
static const QString JumpsNoContactTimeAvrUid = "{C4485A08-65FA-4184-993E-2D236A5CF536}";
static const QString ReactiveForceIndexUid = "{0751A4B2-0269-43C2-B379-48DAEB5BA075}";
}


class DropTestFactors : public ChannelMultifactor
{
public:
    explicit DropTestFactors(const QString &testUid,
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
     * \brief Возвращает название группы показателей для использования, как константу
     */
    static QString nameAsConst();

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
   SignalsDefines::DropJumpFactors jump(const int idx) const;

   /*!
    * \brief Расчет дополнительных показателей дроп теста
    * Входные параметры:
    * \param timeContact - время контактной фазы
    * \param timeNoContact - время бесконтактной фазы
    * \param fallHeight - высота соскакивания
    * \param height - высота прыжка
    * \param massa - масса пациента
    * Рассчитываемые параметры:
    * \param power - мощность
    * \param stiffness - жесткость
    * \param initialSpeed - начальная скорость
    * \param rsi - индекс реактивной силы
    */
   static void calculateAdvFactors(const double timeContact, const double timeNoContact, const double fallHeight,
                                   const double height, const double massa,
                                   double &power, double &stiffness, double &initialSpeed, double &rsi);

private:
   int getPatientMassa();

   double m_time {0};
   QList<SignalsDefines::DropJumpFactors> m_jumps;
};

#endif // DROPTESTFACTORS_H
