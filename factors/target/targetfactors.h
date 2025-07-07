#ifndef TARGETFACTORS_H
#define TARGETFACTORS_H

#include <QObject>
#include <QCoreApplication>

#include "multifactor.h"

namespace TargetFactorsDefines
{
static const QString GroupUid = "{F9EE075C-716E-43F6-91EA-72FD1E921193}";

static const QString TargetScoreUid = "{552B2B27-4F58-436A-BE1E-03978E7EE604}";
static const QString TargetTotalScoreUid = "{89B72141-4978-499B-924B-DB2DAE01E337}";
}

class Stabilogram;

/*!
 * \brief Класс модуля расчета показателей мишени TargetFactors class
 */
class TargetFactors : public ProbeMultifactor
{
public:
    explicit TargetFactors(const QString &testUid,
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

    /*!
     * \brief Возвращает значение отсчета гистограммы
     * \param idx - индекс отсчета (0 - 9)
     */
    int histogram(const int idx) const;

private:

    int getDiap(Stabilogram* stab) const;

    double m_score {0};
    double m_totalScore {0};

    int m_hist[10];    ///< Гистограмма распределения

};

#endif // TARGETFACTORS_H
