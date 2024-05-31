#ifndef PULSEFACTORS_H
#define PULSEFACTORS_H

#include <QObject>
#include <QCoreApplication>
#include <QVector>
#include <QMap>
#include "multifactor.h"

namespace PulseFactorsDefines
{
static const QString GroupUid = "{BAED5445-97FE-48B9-99DF-1A5FF367A870}";
static const QString GroupName = QCoreApplication::tr("Показатели вариационной пульсометрии");

static const QString PulseUid = "{6930C0E6-6858-4BE3-8040-6A9E130FA0E1}";
static const QString IVRUid = "{627AE093-E357-4034-ADED-70A6CF33EE5E}";
static const QString PAPRUid = "{A5665485-3B20-4FB8-8AB9-64771358F1D9}";
static const QString VPRUid = "{E12CBCF8-DE3D-4942-AEFD-98B58BEF52C9}";
static const QString INUid = "{40A48ADD-91D8-4679-8E75-31F963720CA1}";
static const QString ModeUid = "{B3E5291E-444F-42D5-ABDE-F0A0ACC842C7}";
static const QString AModeUid = "{6ECBB464-AF64-4392-A54B-32EA6D1B9026}";
static const QString MxDMnUid = "{527835F4-19CF-49CC-8971-6D360D93BFFF}";
static const QString SDNNUid = "{CED883B8-8F38-41CD-B9D8-2F4200AB87B5}";
static const QString RMSSDUid = "{EEC2F1E5-B527-4ADB-88B1-0E3C75BF4300}";
static const QString pNN50Uid = "{8631AEA7-F9D0-4A1B-8002-D26E07049AEE}";
static const QString CVUid = "{C730DAD5-8658-4B22-BABF-7C5605130105}";
static const QString SDSDUid = "{67DD0651-AA19-4E69-B409-563E9884A812}";
static const QString MDUid = "{F4DD9750-05E1-423C-959C-B3E40B8B428D}";
static const QString AKACClUid = "{0FA4BFE9-6CE7-4DF0-B9E3-78B788057C14}";
static const QString AKACC0Uid = "{C6035C76-DC9C-4380-BB73-E90875A64D12}";
static const QString DXUid = "{7AAC21AF-617B-49E9-B96E-975201D5FD2E}";

static const int iVPGCount = 40;  ///< это для Баевского
static const int iAKPointsCount = 30;
static const int iPNNDifValue = 50;


/*!
 * \brief Суммарный эффект регуляции The OverallEffectReg enum
 */
enum OverallEffectReg
{
      oerSevereTachycardia = -2     ///< выраженная тахикардия
    , oerModerateTachycardia = -1   ///< умеренная тахикардия
    , oerNormosystole = 0          ///< нормосистолия
    , oerModerateBradycardia = 1    ///< умеренная брадикардия
    , oerSevereBradycardia = 2      ///< выраженная брадикардия
};

static QMap<OverallEffectReg, QString> OverallEffectRegName =
{
      std::pair<OverallEffectReg, QString> (oerSevereTachycardia, QCoreApplication::tr("выраженная тахикардия"))
    , std::pair<OverallEffectReg, QString> (oerModerateTachycardia, QCoreApplication::tr("умеренная тахикардия"))
    , std::pair<OverallEffectReg, QString> (oerNormosystole, QCoreApplication::tr("нормосистолия"))
    , std::pair<OverallEffectReg, QString> (oerModerateBradycardia, QCoreApplication::tr("умеренная брадикардия"))
    , std::pair<OverallEffectReg, QString> (oerSevereBradycardia, QCoreApplication::tr("выраженная брадикардия"))
};

/*!
 * \brief Функция автоматизма The AutomaticFunction enum
 */
enum AutomaticFunction
{
      afPronDisturbAuto = -2         ///< нарушение автоматизма выраженное
    , afModerateAutoDisorder = -1    ///< нарушение автоматизма умеренное
    , afModerateSinusArrhythmia = 0  ///< умеренная синусовая аритмия
    , afSevereSinusArrhythmia = 1    ///< выраженная синусовая аритмия
    , afRigidRhythm = 2              ///< ригидный ритм
};

static QMap<AutomaticFunction, QString> AutomaticFunctionName =
{
      std::pair<AutomaticFunction, QString> (afPronDisturbAuto, QCoreApplication::tr("нарушение автоматизма выраженное"))
    , std::pair<AutomaticFunction, QString> (afModerateAutoDisorder, QCoreApplication::tr("нарушение автоматизма умеренное"))
    , std::pair<AutomaticFunction, QString> (afModerateSinusArrhythmia, QCoreApplication::tr("умеренная синусовая аритмия"))
    , std::pair<AutomaticFunction, QString> (afSevereSinusArrhythmia, QCoreApplication::tr("выраженная синусовая аритмия"))
    , std::pair<AutomaticFunction, QString> (afRigidRhythm, QCoreApplication::tr("ригидный ритм"))
};

/*!
 * \brief Вегетативный гомеостаз The VegetativeHomeostasis enum
 */
enum VegetativeHomeostasis
{
      vhSeverePredominancePSNS = -2     ///< выраженное преобладание ПСНС
    , vhModeratePredominancePSNS = -1   ///< умеренное преобладание ПСНС
    , vhSaved = 0                       ///< сохранен
    , vhModeratePredominanceSNS = 1     ///< умеренное преобладание СНС
    , vhSeverePredominanceSNS = 2       ///< выраженное преобладание СНС
};

static QMap<VegetativeHomeostasis, QString> VegetativeHomeostasisName =
{
      std::pair<VegetativeHomeostasis, QString> (vhSeverePredominancePSNS, QCoreApplication::tr("выраженное преобладание ПСНС"))
    , std::pair<VegetativeHomeostasis, QString> (vhModeratePredominancePSNS, QCoreApplication::tr("умеренное преобладание ПСНС"))
    , std::pair<VegetativeHomeostasis, QString> (vhSaved, QCoreApplication::tr("сохранен"))
    , std::pair<VegetativeHomeostasis, QString> (vhModeratePredominanceSNS, QCoreApplication::tr("умеренное преобладание СНС"))
    , std::pair<VegetativeHomeostasis, QString> (vhSeverePredominanceSNS, QCoreApplication::tr("выраженное преобладание СНС"))
};

/*!
 * \brief Устойчивость регуляции The StabilityRegulation enum
 */
enum StabilityRegulation
{
      srDysregulationPredominancePSNS = -2   ///< дисрегуляция с преобладанием ПСНС
    , srTransitionProcess = -1               ///< переходный процесс
    , srSustainableRegulation = 0            ///< устойчивая регуляция
    , srCentralDysregulation = 1             ///< дисрегуляция центрального типа
    , srDominatedDysregulationSNS = 2        ///< дисрегуляция с преобладанием СНС
};

static QMap<StabilityRegulation, QString> StabilityRegulationName =
{
      std::pair<StabilityRegulation, QString> (srDysregulationPredominancePSNS, QCoreApplication::tr("дисрегуляция с преобладанием ПСНС"))
    , std::pair<StabilityRegulation, QString> (srTransitionProcess, QCoreApplication::tr("переходный процесс"))
    , std::pair<StabilityRegulation, QString> (srSustainableRegulation, QCoreApplication::tr("устойчивая регуляция"))
    , std::pair<StabilityRegulation, QString> (srCentralDysregulation, QCoreApplication::tr("дисрегуляция центрального типа"))
    , std::pair<StabilityRegulation, QString> (srDominatedDysregulationSNS, QCoreApplication::tr("дисрегуляция с преобладанием СНС"))
};

}

class Ritmogram;

/*!
 * \brief Класс модуля расчета показателей ритмограммы The PulseFactors class
 */
class PulseFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    PulseFactors(const QString &testUid,
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
     * \brief Доступ к границам статистики
     */
    int statBoundsCount() const;
    double statBounds(const int idx) const;

    /*!
     * \brief Доступ к счетчикам статистики
     */
    int statCountsCount() const;
    double statCounts(const int idx) const;

    /*!
     * \brief Доступ к коэффициентам корреляции
     * \param idx - номер коэффициента корреляции 0 - PulseFactorsDefines::iAKPointsCount
     * \return коэффициент корреляции
     */
    double correlationCoef(const int idx);

    /*!
     * \brief Возвращает значение суммарного эффекта регуляции
     */
    PulseFactorsDefines::OverallEffectReg overallEffectReg() {return  m_summReg;}
    /*!
     * \brief Возвращает значение функции автоматизма
     */
    PulseFactorsDefines::AutomaticFunction automaticFunction() {return  m_funcAuto;}

    /*!
     * \brief Возвращает значение вегетативного гомеостаза
     */
    PulseFactorsDefines::VegetativeHomeostasis vegetativeHomeostasis() {return m_vegHomst;}
    /*!
     * \brief Возвращает значение устойчивости регуляции
     */
    PulseFactorsDefines::StabilityRegulation stabilityRegulation() {return m_ustReg;}

private:
    void assignStat(const double sdt, const double tMin, const double tMax);
    void addRRValue(const double val, const double t);
    void finalCalculate();  /// Done;
    void processHist();

    /*!
     * \brief Генерация заключения
     */
    void computeConslution();
    /*!
     * \brief Расчет параметров автокорреляции
     */
    void computeAKA(Ritmogram* signal);

    int m_begin {0};  ///< Начальная точка расчета
    int m_end {0};    ///< Конечная точка расчета

    double m_pulse {0};
    double m_RMSSD {0};
    int m_NN50 {0};
    double m_pNN50;
    double m_meanD1 {0}, m_meanD2 {0};
    double m_SDSD {0};
    double m_CVR {0};
    double m_HRM {0};
    double m_MDNN {0};

    // стандартная статистика по интервалам
    // Момент 1,2 (для SDNN)
    double m_meanNN {0}, m_mean2NN {0};
    double m_SDNN {0};

    double m_minNN {INT_MAX};
    double m_maxNN {0};
    double m_prevNN {-1};
    int m_nCnt {0};   ///< Счетчик данных
    int m_nD2Cnt {0}; ///< Количество пар интервалов

    double m_BASC {0};    ///< Коэф регрессии скаттерограммы (a)
    double m_BSC {0};     ///< Коэф угла наклона линии регрессии скаттерограммы (y = b*x + a)
    double m_BCSC {0};    ///< Коэф регрессии для нормали (уравнение b*y - x = c)
    double m_bscC11 {0}, m_bscC12 {0}, m_bscC21 {0}, m_bscY1 {0}, m_bscY2 {0};
    double m_bscCN {0}, m_bscYV2{0};

    QVector<double> m_bStatBnd;  ///< Границы статистики
    QVector<int> m_bStatCnt;     ///< Счетчики статистики
    int m_outerMin {0}, m_outerMax {0};  ///< То, что не попало
    int m_statSize {0};                  ///< Кол-во интервалов

    double m_xMod {0};
    double m_ampMod {0};  ///< Амплитуда моды
    double m_MxDMn {0};   ///< Разность между макс. и мин. значениями
    double m_DXRange {0}; ///< Вариационный размах (по гистограмме)  -> <-
    double m_ACK1 {0};    ///< Первый коэффициент автокоррел. функции
    double m_AKSh0 {0};   ///< Число сдвигов автокор. функции до нуля
    double m_IVR {0}, m_VPR {0}, m_PAPR {0}, m_INNPR {0};
    double m_SIM {0}, m_PAR {0}; ///< Коэффициенты

    double m_masKK[PulseFactorsDefines::iAKPointsCount]; ///< Массив коэффициентов корреляции
    ///< Результаты
    PulseFactorsDefines::OverallEffectReg m_summReg {PulseFactorsDefines::oerNormosystole};
    PulseFactorsDefines::AutomaticFunction m_funcAuto {PulseFactorsDefines::afModerateSinusArrhythmia};
    PulseFactorsDefines::VegetativeHomeostasis m_vegHomst {PulseFactorsDefines::vhSaved};
    PulseFactorsDefines::StabilityRegulation m_ustReg {PulseFactorsDefines::srSustainableRegulation};

};

#endif // PULSEFACTORS_H
