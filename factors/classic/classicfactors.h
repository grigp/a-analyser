#ifndef CLASSICFACTORS_H
#define CLASSICFACTORS_H

#include <QObject>
#include "multifactor.h"


namespace ClassicFactorsDefines
{
static const QString GroupUid = "{CE4B0A70-953E-486E-B065-54BB2854BB5D}";
static const QString MoXUid = "{D2E9D021-07F5-49D4-967F-4AB7E05558D0}";
static const QString MoYUid = "{ADEA0860-76A3-4491-A3FB-BE130E1C64DA}";
static const QString QXUid = "{48DB78C0-EDBD-4065-9CC1-F767499B8C90}";
static const QString QYUid = "{4184ED12-B318-4D95-892F-4B20904E2503}";
static const QString RUid = "{E860357D-8493-43F6-9AC4-49A8557D0F15}";

}


/*!
 * \brief Класс модуля расчета классических  показателей ClassicFactors class
 */
class ClassicFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    explicit ClassicFactors(const QString &testUid,
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

private:

    ///< Показатели
    double m_mx {0};
    double m_my {0};
    double m_qx {0};
    double m_qy {0};
    double m_r {0};
};

///<------------------------------------------------------------------------------------
class ClassicFactorsCreator: public ChannelFactorsCreator
{
public:
    ClassicFactorsCreator() {}
    ChannelMultifactor* createFactors(const QString &testUid,
                                      const QString &probeUid,
                                      const QString &channelId,
                                      QObject *parent = nullptr) override
    {return new ClassicFactors(testUid, probeUid, channelId, parent);}

    bool isValid(const QString &testUid,
                 const QString &probeUid,
                 const QString &channelId) const override
    {
        return ClassicFactors::isValid(testUid, probeUid, channelId);
    }

};

#endif // CLASSICFACTORS_H
