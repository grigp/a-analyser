#ifndef MWSTICKFORCEFACTORS_H
#define MWSTICKFORCEFACTORS_H

#include <QObject>
#include <QCoreApplication>
#include <QVector>
#include <QMap>

#include "multifactor.h"

namespace MWStickForceFactorsDefines
{
static const QString GroupUid = "{AECF23D7-D0FB-4AB2-923B-26D428ED350B}";

static const QString MaxForceUid = "{68BBDD65-E800-4117-B73F-D3D5AABB5ABE}";
static const QString AverageForceUid = "{A677EA17-61B8-4B17-AE9E-55E23A606936}";
}

/*!
 * \brief Класс модуля расчета показателей усилий соревновательной палки The MWStickForceFactors class
 */
class MWStickForceFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    MWStickForceFactors(const QString &testUid,
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
    int m_begin {0};  ///< Начальная точка расчета
    int m_end {0};    ///< Конечная точка расчета

    double m_maxForce {0};
    double m_averageForce {0};

};

#endif // MWSTICKFORCEFACTORS_H
