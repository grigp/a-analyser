#ifndef PULSESPECTRFACTORS_H
#define PULSESPECTRFACTORS_H

#include <QCoreApplication>
#include <QObject>
#include <QVector>

#include "multifactor.h"

namespace PulseSpectrFactorsDefines
{
static const QString GroupUid = "{892FF592-6DF2-4E19-B3C6-7DA8D8E8C5DC}";

static const QString PwHFUid = "{4ADE99DE-486D-4ED6-A608-5C5758FB53F2}";
static const QString PwLFUid = "{C33BDC30-5B21-46C0-8B1C-77D2BC0D2862}";
static const QString PwVLFUid = "{63C18C71-7A08-47D5-BA3E-07D620465E00}";
static const QString PwULFUid = "{CA1D24E3-30B5-4B84-A93D-5957CE0C3E72}";
static const QString PwSumUid = "{08EC2EFB-1F6C-4C2C-A40B-257AA43CCC85}";
static const QString PrHFUid = "{FDFA7F27-5FE1-4225-9AE4-EE596A09E71A}";
static const QString PrLFUid = "{A6A0B48E-C966-4E1B-B59A-9492049B781C}";
static const QString PrVLFUid = "{B433F5FF-6971-41F8-BF4F-2A78BBB69E20}";
static const QString PrULFUid = "{BB51B485-BC33-425C-B040-84A1E9BDB913}";
static const QString PwAvHFUid = "{45FCE29C-48E7-432C-A026-73C629032AD5}";
static const QString PwAvLFUid = "{ECCA8A98-CADC-4F5A-997F-F476CB3713E6}";
static const QString PwAvVLFUid = "{C1944C42-73C0-4334-9EFF-FCC5F08B8C29}";
static const QString PwAvULFUid = "{44A28439-5A84-4DCD-909A-B917C90C76EA}";
static const QString LHUid = "{42300DE0-D834-44BB-A76A-179397AA180E}";
static const QString Pw60Uid = "{CC3F4330-81B2-4C26-B4C0-88d14A6FB127}";
static const QString CentIdxUid = "{B26CCBFB-912C-4917-8F9C-F2A843B1221D}";

static const QString FqMidSpUid = "{F94D45A4-DCB9-46AA-865D-457BE8ADFE53}";
static const QString LFnuUid = "{6EEBDD7B-16D5-4956-B4B4-5ED7296B9E06}";
static const QString HFnuUid = "{F4201ED5-540C-4487-A515-D6FE490E6F8E}";
}

class PulseSpectrFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    PulseSpectrFactors(const QString &testUid,
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

    /*!
     * \brief Возвращает кол-во точек спектра
     */
    int spectrCount() const;

    /*!
     * \brief Возвращает значение в точке спектра
     * \param idx - индекс значения (0 - spectrCount())
     */
    double spectrValue(const int idx) const;

    /*!
     * \brief Возвращает навязанную частоту дискретизации, используемую для расчета спектра ритмограммы.
     * Так как отсчеты идут нерегулярно, частота равна среднему значению длительности R-R интервала.
     * Для расчета частоты отсчета спектра, надо воспользоваться формулой: f = (i * freqRate()) / spectrCount()
     */
    double freqRate() const {return m_freqRate;}

    /*!
     * \brief Возвращает длительность сигнала
     */
    double duration() {return m_duration;}

private:
    QVector<double> m_spectr;
    double m_freqRate {1};

    double m_duration {0};

    //! Показатели
    double m_PwHF {0};
    double m_PwLF {0};
    double m_PwVLF {0};
    double m_PwULF {0};
    double m_PwSum {0};
    double m_PrHF {0};
    double m_PrLF {0};
    double m_PrVLF {0};
    double m_PrULF {0};
    double m_PwAvHF {0};
    double m_PwAvLF {0};
    double m_PwAvVLF {0};
    double m_PwAvULF {0};
    double m_LH {0};
    double m_Pw60 {0};
    double m_CentIdx {0};
    double m_FqMidSp {0};
    double m_LFnu {0};
    double m_HFnu {0};

};

#endif // PULSESPECTRFACTORS_H
