#ifndef SPECTRSINGLESIGNALFACTORS_H
#define SPECTRSINGLESIGNALFACTORS_H

#include <QObject>
#include <QCoreApplication>

#include "multifactor.h"

namespace SpectrSingleSignalFactorsDefines
{
static const QString GroupUid = "{BF8E3564-6014-470F-AF8D-7EC928FBBA3B}";
static const QString GroupName = QCoreApplication::tr("Показатели спектра одиночного сигнала");

static const QString Frequency1Uid = "{A45D3563-309E-4A4B-9FAA-6F7FC08DB2DE}";
static const QString Amplitude1Uid = "{ED586057-00DF-43EE-AE2C-6DCFD528141C}";
static const QString Frequency2Uid = "{52328AA3-8B78-4D5E-8C18-B21291D51588}";
static const QString Amplitude2Uid = "{B7B21388-5124-405F-9679-C6226BB05DDF}";
static const QString Frequency3Uid = "{2D2D01E9-BD66-413E-B0D7-BC58DE687DC2}";
static const QString Amplitude3Uid = "{A1493B87-52B3-4533-B511-59EF280A0905}";
static const QString Level60Uid = "{04E80CD2-67AF-48FD-AE90-5EB39EB15590}";
static const QString Power1Uid = "{58AA9D94-3114-4357-906D-02536F8E972B}";
static const QString Power2Uid = "{B6E5DC97-8314-471E-A0D6-E946156C88C6}";
static const QString Power3Uid = "{0BC53416-43F2-49F5-8C49-FCC58243CA58}";

}

class SignalFFT;

/*!
 * \brief Класс модуля расчета показателей спектра одиночного сигнала The SpectrSingleSignalFactors class
 */
class SpectrSingleSignalFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    explicit SpectrSingleSignalFactors(const QString &testUid,
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

private:
    struct FactorValues
    {
        double freq1 {0};
        double ampl1 {0};
        double freq2 {0};
        double ampl2 {0};
        double freq3 {0};
        double ampl3 {0};

        double pwr60 {0};

        double power1 {0};
        double power2 {0};
        double power3 {0};
    };

    void computeFactors();
    void addFactors();

    SignalFFT *m_spectr {nullptr};
    int m_frequency {50};
    FactorValues m_values;

};

#endif // SPECTRSINGLESIGNALFACTORS_H
