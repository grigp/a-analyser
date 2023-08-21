#ifndef OCTAEDRONFACTORS_H
#define OCTAEDRONFACTORS_H

#include <QObject>

#include "basedefines.h"
#include "multifactor.h"

namespace OctaedronFactorsDefines
{
static const QString GroupUid = "{E5409C1B-045B-4278-96A7-B872A42E3453}";
static const QString GroupName = QCoreApplication::tr("Показатели тренажера \"Октаэдр\"");

//static const QString QualityAverageUid = "{76C530D6-7CB7-4BDC-982D-23D7B223251C}";
static const QString QualityUUid = "{850740BA-701F-4B06-BDBF-57B607D95150}";
static const QString QualityULUid = "{86840ECF-E8DB-421C-AA71-5AE014B9530C}";
static const QString QualityLUid = "{B604A3F7-213C-4E32-9D4C-60E475969824}";
static const QString QualityDLUid = "{FBDE12E4-E936-442B-A3C8-31E513D7D197}";
static const QString QualityDUid = "{7CC9E337-F543-40A3-ACD9-C646BF511F81}";
static const QString QualityDRUid = "{3CC89FA5-3712-4ACC-BE03-AD099AB87A4D}";
static const QString QualityRUid = "{E561C008-B2E2-4F53-9150-726F1C89FA54}";
static const QString QualityURUid = "{59B54B19-A2C1-4303-B1A5-CF57EDB9C268}";
}

class OctaedronResultData;

class OctaedronFactors : public ProbeMultifactor
{
public:
    explicit OctaedronFactors(const QString &testUid,
                              const QString &probeUid,
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
                        const QString &probeUid);

    /*!
     * \brief Рассчитывает показатели для теста с заданным uid
     * uid теста задается в конструкторе
     */
    void calculate() override;

    /*!
     * \brief Регистрирует факторсгруппу и показатели
     */
    static void registerFactors();

    int stageTime() const;
    QString circeRoundRuleMode() const;
    QString directionMode() const;
    int radius() const;
    int freq() const;
    int diap() const;

private:
    void getEventLablels();

    void calculateBaseFactors();

    BaseDefines::CirceRoundRuleMode getCRRM();

    OctaedronResultData* m_resData {nullptr};
    double m_values[8] {0, 0, 0, 0, 0, 0, 0, 0};
};

#endif // OCTAEDRONFACTORS_H
