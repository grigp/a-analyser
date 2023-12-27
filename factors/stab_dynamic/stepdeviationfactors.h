#ifndef STEPDEVIATIONFACTORS_H
#define STEPDEVIATIONFACTORS_H

#include <QVector>

#include "basedefines.h"
#include "multifactor.h"

namespace StepDeviationFactorsDefines
{
static const QString GroupUid = "{9055B2C8-931C-464C-A8A1-175FF03CE61D}";
static const QString GroupName = QCoreApplication::tr("Показатели теста прироста");

static const QString TimeUid = "{73BA4F98-7DCF-450F-BED2-B87905CEFC24}";
static const QString StepCountUid = "{73D64914-414D-4A41-832F-2348976006B5}";
static const QString ErrorCountUid = "{3ABD6C33-3BEF-4ED1-881E-E79A697E9D43}";
static const QString GrowthAvrgUid = "{4E87C53D-1D18-4F80-97B9-B2A63644D00C}";
static const QString TimeAvrgUid = "{EA78B3B9-9109-49D7-B031-BB4112A50D1D}";
static const QString SensitivityUid = "{5911C6E3-24B4-401C-B403-AF753F186AAE}";
static const QString MaxDeviationUid = "{BD7D98D9-CBF8-49BE-B2AA-F587C6DD8A4F}";
static const QString DifferenceUid = "{F8EA6499-1F8B-4C22-BEAC-9F74DA745154}";
static const QString TimeIncreaseAwrgUid = "{F0727E66-DC05-4E04-AE91-D8A362423339}";
static const QString TimeReturnAwrgUid = "{6D1C47D3-2AA7-4EC5-A8E0-EAA0A54E8B68}";
static const QString MinExtrTimeUid = "{7A7EE4EC-D65B-4AC2-A288-E453D53BF841}";
static const QString MaxExtrTimeUid = "{05A072F6-24DD-4C09-A472-61A72A7ACE3C}";
static const QString PickCountUid = "{6D93715E-277C-421C-81C7-984F6F60449B}";
static const QString PickCountAvrgUid = "{A821D9AC-67B5-4BB3-A5A1-A299D41B51B2}";
static const QString MaxPickCountUid = "{257D2FFE-5534-437A-89A3-5AFED760E812}";
static const QString MinPickCountUid = "{3C306236-A022-48EF-8C11-9B5286D178E9}";
static const QString ExtrPickCountUid = "{6B185805-E0C4-45D4-8C49-1AFF7710F136}";
static const QString AmplSemiWaveUid = "{5A27ACFC-C52C-45A1-B880-8CFE2D9B14C5}";
static const QString TimeSemiWaveUid = "{6C831AB6-561A-45F4-99C6-597284F28FA6}";
static const QString RitmUid = "{0659388D-9780-4161-B9B3-19331B912651}";
static const QString RitmStabUid = "{69E11637-C684-4FAA-A652-1117697A7F03}";
}

/*!
 * \brief Класс показателей теста прироста (со ступенчатым отклонением) StepDeviatonFactors class
 */
class StepDeviationFactors : public ProbeMultifactor
{
public:
    explicit StepDeviationFactors(const QString &testUid,
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

    /*!
     * \brief Возвращает размер сигнала
     */
    int signalSize() const;

    /*!
     * \brief Возвращает частоту дискретизации сигнала
     */
    int signalFreqency() const {return m_freq;}

    /*!
     * \brief Возвращает значение сигнала по индексу
     * \param idx - индекс
     */
    double signal(const int idx) const;

    /*!
     * \brief Возвращает значение фильтрованного сигнала по индексу
     * \param idx - индекс
     */
    double signalFiltred(const int idx) const;

    int growthDynCount() const;
    double growthDynValue(const int idx) const;

    int lengthDynCount() const;
    double lengthDynValue(const int idx) const;

    /*!
     * \brief Возвращает направление отклонений
     */
    BaseDefines::Directions direction() const {return m_direction;}

private:
    /*!
     * \brief Чтение сигналов, выбор нужного канала, фильтрация
     */
    void readSignal();

    /*!
     * \brief Расчет участков сигнала
     */
    void assignSections();

    /*!
     * \brief Расчет основных показателей
     */
    void computeFactorsMain();

    /*!
     * \brief Расчет показателей экстремумов
     */
    void computeFactorsExtremums();

    /*!
     * \brief Расчет показателей, связанных с пиками для участка сигнала
     * \param begin - начало участка
     * \param end - конец участка
     * \param pickCount - возвращаемое значение кол-ва пиков
     * \param ampl - возвращаемое значение амплитуды полуволны
     * \param time - возвращаемое значение длительности полуволны
     */
    void computePicks(const int begin, const int end, int &pickCount, double &ampl, double &time);

    /*!
     * \brief Возвращает матожидание и стандартное отклонение для первой секунды сигнала
     * \param m, q - матожидание и стандартное отклонение
     */
    void computeStartPosition(double &m, double &q);

    /*!
     * \brief добавление показателей
     */
    void addFactors();

    QVector<double> m_signal;       ///< Отнормированный сигнал нужного канала исходный
    QVector<double> m_signalFlt;    ///< Отнормированный сигнал нужного канала фильтрованный
    int m_freq {0};

    BaseDefines::Directions m_direction {BaseDefines::dirUp};
    QList<BaseDefines::Extremum> m_extrList;  ///< Список экстремумов
    QList<BaseDefines::Section> m_Extr;       ///< Список участков сигнала экстремумов
    QList<BaseDefines::Section> m_Trans;      ///< Список участков сигнала переходов

    QList<double> m_growthDynamic;  ///< Динамика прироста
    QList<double> m_lengthDynamic;  ///< Динамика длительности отклонений

    double m_time {0};
    double m_stepCount {0};
    double m_errorCount {0};
    double m_growthAvrg {0};
    double m_timeAvrg {0};
    double m_sensitivity {0};
    double m_maxDeviation {0};
    double m_difference {0};
    double m_timeIncreaseAwrg {0};
    double m_timeReturnAwrg {0};
    double m_minExtrTime {0};
    double m_maxExtrTime {0};
    double m_pickCount {0};
    double m_pickCountAvrg {0};
    double m_maxPickCount {0};
    double m_minPickCount {0};
    double m_extrPickCount {0};
    double m_amplSemiWave {0};
    double m_timeSemiWave {0};

};

#endif // STEPDEVIATIONFACTORS_H
