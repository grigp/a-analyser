#ifndef JUMPHEIGHTSINGLEFACTORS_H
#define JUMPHEIGHTSINGLEFACTORS_H

#include <QObject>
#include <QCoreApplication>

#include "multifactor.h"

namespace JumpHeightSingleFactorsDefines
{
static const QString GroupUid = "{0F0CAD79-7A47-469D-819F-34A48FCBCD82}";
static const QString GroupName = QCoreApplication::tr("Показатели высоты одиночного прыжка");
static const QString JumpHeightUid = "{6A7E2237-49BE-4DF3-AE32-8E32E82208D3}";
}


/*!
 * \brief Класс показателей высоты прыжка JumpHeightFactors class
 */
class JumpHeightSingleFactors : public ChannelMultifactor
{
public:
    explicit JumpHeightSingleFactors(const QString &testUid,
                                     const QString &probeUid,
                                     const QString &channelId,
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
};

#endif // JUMPHEIGHTSINGLEFACTORS_H
