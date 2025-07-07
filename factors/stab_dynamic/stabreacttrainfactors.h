#ifndef STABREACTTRAINFACTORS_H
#define STABREACTTRAINFACTORS_H

#include <QObject>

#include "basedefines.h"
#include "multifactor.h"

namespace StabReactTrainFactorsDefines
{
static const QString GroupUid = "{7256EC87-60AB-4815-B03B-07B0E1EC9FEC}";

static const QString LatentAvrUid = "{B121111A-D736-4574-9985-FF485C3E0604}";
static const QString LatentUUid = "{83D66F37-5E31-4CCE-8D45-10D42C6910DD}";
static const QString LatentULUid = "{9D304B37-E0D0-4978-A810-3B69FD03959E}";
static const QString LatentLUid = "{CC6FBEC3-A3DE-44D9-9AD4-EA606BF63636}";
static const QString LatentDLUid = "{2164DC81-35DE-4796-8D97-3DC3D84E4340}";
static const QString LatentDUid = "{f8AD548C-F20B-4030-9C33-68228FA362F9}";
static const QString LatentDRUid = "{F233B0B2-8A22-4D54-AC94-E60884848E1C}";
static const QString LatentRUid = "{69591303-101B-4479-B4BF-5801EACD75B3}";
static const QString LatentURUid = "{D88B3858-CB0A-4723-82DE-C327AA66D11A}";

static const QString TimeAvrUid = "{82D90EBD-6591-4C7F-AF84-1000609BB9C9}";
static const QString TimeUUid = "{2A524304-F2A2-4B0A-8D58-A7F2E10F1D85}";
static const QString TimeULUid = "{5E87C9B2-3D3F-4DAF-B59A-32ABC1D5B9E1}";
static const QString TimeLUid = "{7D850DA4-1156-45A9-AE58-64AD2032CEFE}";
static const QString TimeDLUid = "{82CEFCB8-DB10-4A83-B83A-D1062CFE0787}";
static const QString TimeDUid = "{91ADAE98-BAEB-4FC1-AD64-199BD7E216C4}";
static const QString TimeDRUid = "{58D8E0C7-AFC2-4A05-81ED-F7E33AFA67E7}";
static const QString TimeRUid = "{EDABA897-7B87-4467-B925-6002D401FE78}";
static const QString TimeURUid = "{4AE0DD70-8CE5-4B8B-B14F-9AA9B334F8D2}";
}

class OctaedronResultData;


/*!
 * \brief Класс показателей тренажера статокинетической реакции The StabReactTrainFactors class
 */
class StabReactTrainFactors : public ProbeMultifactor
{
public:
    explicit StabReactTrainFactors(const QString &testUid,
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

    /*!
     * \brief Возвращает латентный период для этапа по индексу
     */
    double latent(const int idx) const;

    /*!
     * \brief Возвращает время реакции для этапа по индексу
     */
    double time(const int idx) const;

    double latentAvg() const {return m_valLatentAvg;}
    double timeAvg() const {return m_valTimeAvg;}

private:
    void getEventLablels();

    void calculateBaseFactors();

    BaseDefines::CirceRoundRuleMode getCRRM();

    OctaedronResultData* m_resData {nullptr};
    double m_valLatent[8] {0, 0, 0, 0, 0, 0, 0, 0};
    double m_valTime[8] {0, 0, 0, 0, 0, 0, 0, 0};
    double m_valLatentAvg {0};
    double m_valTimeAvg {0};
};

#endif // STABREACTTRAINFACTORS_H
