#ifndef TRIANGLEFACTORS_H
#define TRIANGLEFACTORS_H

#include <QObject>

#include "baseutils.h"
#include "multifactor.h"
#include "signalsdefines.h"

namespace TriangleFactorsDefines
{
static const QString GroupUid = "{262C7BC3-2EFA-40CA-8D08-D770E76F57FC}";
static const QString LUpUid = "{E16E7852-9E22-4385-8A3A-B949C27505BB}";
}

class TriangleFactors : public ProbeMultifactor
{
public:
    explicit TriangleFactors(const QString &testUid,
                             const QString &probeUid,
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

    int diap() const ;
};

#endif // TRIANGLEFACTORS_H
