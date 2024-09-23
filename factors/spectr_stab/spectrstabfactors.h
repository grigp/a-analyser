#ifndef SPECTRSTABFACTORS_H
#define SPECTRSTABFACTORS_H

#include <QObject>
#include <QCoreApplication>

#include "multifactor.h"

namespace SpectrStabFactorsDefines
{
static const QString GroupUid = "{82E05D05-B591-4468-9873-3D028002E293}";
static const QString GroupName = QCoreApplication::tr("Показатели спектра стабилограммы");

namespace Frontal {
static const QString Frequency1Uid = "{7FED3AD3-5679-492C-9D73-607995084F9C}";
static const QString Amplitude1Uid = "{F0A36025-BF28-4100-BF68-ABF121559DF6}";
static const QString Frequency2Uid = "{FF5A89C1-EEE8-4696-B500-6E807C8F6AD5}";
static const QString Amplitude2Uid = "{7E862D1B-C48F-4DF8-8C4E-4889F3A70429}";
static const QString Frequency3Uid = "{64559081-9CBC-477E-967D-22F565EB7D47}";
static const QString Amplitude3Uid = "{BE20023B-D01F-48C5-81CC-8B289537E29D}";
static const QString Level60Uid = "{98B15EDF-07BD-4EC2-B9D6-E7C6B02A8F14}";
static const QString Power1Uid = "{D882381F-16B5-4725-93CF-129DD9597A6A}";
static const QString Power2Uid = "{B70CD120-4DAD-4E0B-968A-90530B8296A8}";
static const QString Power3Uid = "{6E9CE55A-8782-4711-B324-E3C498BA9967}";
}

namespace Sagittal {
static const QString Frequency1Uid = "{958EA680-974D-4010-8D38-75E0108BC616}";
static const QString Amplitude1Uid = "{9C24A6B0-B117-4F7E-AF2D-7412943B59F7}";
static const QString Frequency2Uid = "{88CC6D21-BD04-4542-AB61-C768086BC0DE}";
static const QString Amplitude2Uid = "{F0FBE53D-FA83-4FB5-B2DD-39CB2F54DB21}";
static const QString Frequency3Uid = "{5ED4743B-9848-40F0-A04E-3CDA1FABDD4D}";
static const QString Amplitude3Uid = "{ADC977F4-AC72-4E66-AD01-BB6846E22654}";
static const QString Level60Uid = "{6266872C-F81F-406A-BE5C-1FB82BFD4DCB}";
static const QString Power1Uid = "{6F576287-A56E-47E6-9E69-4EFEE8497A40}";
static const QString Power2Uid = "{7E647012-1991-4B32-A6CB-2DA4652F33F7}";
static const QString Power3Uid = "{6183D43D-345D-42C1-AB36-104022C0DB58}";
}

}

class SignalFFT;

/*!
 * \brief Класс модуля расчета показателей спектра стабилограммы The SpectrStabFactors class
 */
class SpectrStabFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    explicit SpectrStabFactors(const QString &testUid,
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
     * \brief Возвращает кол-во подканалов (рассчитанных спектров)
     */
    int channelsCount() const;

    /*!
     * \brief Возвращает кол-во точек в спектре
     */
    int points() const;

    /*!
     * \brief Возвращает значение спектра
     * \param spectr - номер подканала
     * \param point - номер точки (0 .. m_points-1)
     */
    double value(const int spectr, const int point) const;

    /*!
     * \brief Возвращает чатсоту дискретизации сигнала. Это необходимо для расчета частот
     */
    int frequency() const {return m_frequency;}

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
    void computeFactorsChan(const int chan, FactorValues& values);
    void addFactors();

    SignalFFT *m_spectr {nullptr};
    int m_frequency {50};
    int m_freqMax {50};
    FactorValues m_valuesX, m_valuesY;

};

#endif // SPECTRSTABFACTORS_H
