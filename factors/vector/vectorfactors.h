#ifndef VECTORFACTORS_H
#define VECTORFACTORS_H

#include <QObject>
#include "multifactor.h"

namespace VectorFactorsDefines
{
static const QString GroupUid = "{EE47A8A0-2C55-4B8B-AF44-B642932ACC90}";
static const QString KFRUid = "{1A32C0D6-7A87-4E92-B1EE-ED0BB506EABA}";
static const QString NPVUid = "{A3EE312D-630B-423C-B21B-811FE8E6AD04}";
static const QString KRINDUid = "{92790B4F-DAAA-4666-8ECC-61017B6E0095}";
static const QString VMidUid = "{A3D24C31-7330-4AE7-A0B3-3F1CEED27361}";
static const QString AmplVUid = "{C798791A-AEBB-41F7-9DFC-DA829105EE7E}";

}



class VectorFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    VectorFactors(const QString &testUid,
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

#endif // VECTORFACTORS_H
