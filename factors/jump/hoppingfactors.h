#ifndef HOPPINGFACTORS_H
#define HOPPINGFACTORS_H

#include <QObject>
#include<QCoreApplication>

#include "multifactor.h"

namespace HoppingFactorsDefines
{
static const QString GroupUid = "{1B7E2B3D-A822-421B-9B96-A9CDD1236010}";
static const QString GroupName = QCoreApplication::tr("Показатели соскакивания на платформу");

static const QString JumpHeightUid = "{47C0EEAF-C55E-4A8A-9296-08F747AED039}";
static const QString TimeUid = "{B1F4BAF9-9B81-4231-ACE9-A499353AA572}";
}

/*!
 * \brief Класс расчета показателей соскакивания на платформу HoppingFactors class
 */
class HoppingFactors : public ChannelMultifactor
{
public:
    explicit HoppingFactors(const QString &testUid,
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

private:
   double m_height {0};   ///< Высота прыжка
   double m_time {0};     ///< Время на платформе
};

#endif // HOPPINGFACTORS_H
