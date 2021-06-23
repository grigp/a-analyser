#ifndef STEPOFFSETFACTORS_H
#define STEPOFFSETFACTORS_H

#include <QObject>

#include "baseutils.h"
#include "multifactor.h"
#include "signalsdefines.h"
#include "crossdefines.h"

namespace StepOffsetFactorsDefines
{
static const QString GroupUid = "{917371CA-75C2-4534-8A61-9E2BCA750D17}";

namespace Compensation {
static const QString LatentUid = "{8B7BBD20-95B7-4B77-BDA1-C0D36F076433}";
static const QString SwingTimeUid = "{8D1139FB-F6B9-475C-999E-C550B5725694}";
static const QString SwingAmplUid = "{952A5859-7B74-47E9-8C46-C189CFD87DBA}";
static const QString SwingSpeedUid = "{C94EAE3B-E3D4-4BB3-BD89-7E7B080665B4}";
static const QString SpurtTimeUid = "{EDFA323A-E8FD-48C0-BAF7-BDFF27199537}";
static const QString SpurtAmplUid = "{5C17A435-B27A-4395-8F17-3F98921CA2F3}";
static const QString SpurtSpeedUid = "{B89AB55E-39E2-47BF-909E-56B33499C940}";    //! Скорость броска в %/сек
static const QString SpurtSpeedMMUid = "{66D66A48-ECEE-47C6-9B19-2DA3BEE1EEAF}";  //! Скорость броска в мм/сек
static const QString StatismUid = "{B250F6C6-673A-4CD4-976C-E26BF0294E99}";
static const QString ReactionTimeUid = "{CB979F2B-69B9-4EC2-92C7-68A8A5CDBD79}";
static const QString OvershootAmplUid = "{CA59CBB4-D865-4646-B888-CD49116FDF47}";
static const QString StabilityDeviationUid = "{6F71EC53-27BE-4B5F-BBE5-91D665EB7322}";
static const QString RetentionDeviationUid = "{94F40847-F855-4383-B89A-62B8F5BE1E8B}";
static const QString ProcessKindUid = "{5B9B91C2-07E2-47CD-BDAF-1E4DBEFE3990}";
static const QString CorrectMotorTimeUid = "{A26F23B4-9068-49C2-AC5D-8295B4E0771C}";
static const QString CorrectMotorAmplUid = "{2B593BDC-8245-4DDE-AFB8-E996667281B8}";
static const QString CorrectMotorPowerUid = "{399FBD06-9BDA-4DA3-8B1A-57CBD814CBAF}";
static const QString CorrectMotorErrorUid = "{AA96DB5E-5DB9-4D88-8945-F93BDA0A900C}";
static const QString CorrectKognTimeUid = "{13C83BF5-98C4-4D05-93B0-D7A580BDEAF6}";
static const QString CorrectKognAmplUid = "{E1128E46-2280-49CE-BB28-286B7048BA6A}";
static const QString CorrectKognPowerUid = "{898D009A-FB31-43B9-9205-9BB977F0BC3B}";
static const QString CorrectKognErrorUid = "{1622A98F-023F-4522-B3B7-0AD596D5C68E}";
}

namespace Return {
static const QString LatentUid = "{4887C572-597C-4F60-ABF5-BD4039D43A4E}";
static const QString SwingTimeUid = "{0Bf9CA8A-FD60-4C56-9C06-D8C34B46155F}";
static const QString SwingAmplUid = "{E6264A55-2346-420D-ACB5-BEEFB9CDA308}";
static const QString SwingSpeedUid = "{30F7B7B1-25A2-497F-A42E-76F990946508}";
static const QString SpurtTimeUid = "{EE4FDCD7-5D9D-4FD1-A500-66388DEB4b42}";
static const QString SpurtAmplUid = "{A731FE70-377A-4895-826F-DE6F5F39666A}";
static const QString SpurtSpeedUid = "{A2BE2D43-39DF-4E27-9DF9-B824084641FB}";    //! Скорость броска в %/сек
static const QString SpurtSpeedMMUid = "{BB562FE3-5713-4C77-8CAC-7CDD574C7BB0}";  //! Скорость броска в мм/сек
static const QString StatismUid = "{0A7C2039-836A-4CA4-ADC6-B366A046F7DD}";
static const QString ReactionTimeUid = "{4D18B915-DBFD-402F-A18A-861780FAC20C}";
static const QString OvershootAmplUid = "{40AD2C39-F9A1-4243-A749-BB1EC6380EBB}";
static const QString StabilityDeviationUid = "{38AED5E6-BEF0-43A0-82E6-7666F85B86D1}";
static const QString RetentionDeviationUid = "{7DEE3F59-FE45-441A-8A57-2580FBC82C17}";
static const QString ProcessKindUid = "{FCE6B18D-6D36-4F84-8EBF-4109EC20E5D1}";
static const QString CorrectMotorTimeUid = "{14BE7D31-5C0B-4309-9E8B-88CCA7FD982B}";
static const QString CorrectMotorAmplUid = "{28CEF856-3F82-4EAC-ADB6-9A6ADC8EEC5D}";
static const QString CorrectMotorPowerUid = "{0358D094-FAB7-4AA3-811B-342A04C17F12}";
static const QString CorrectMotorErrorUid = "{90A2E978-8727-4D3C-9E47-F2AFAF481115}";
static const QString CorrectKognTimeUid = "{67280DFC-507C-49D0-9D3D-4D7D659B5DF3}";
static const QString CorrectKognAmplUid = "{16A99D70-1046-4AA9-A78B-EC874CF07A16}";
static const QString CorrectKognPowerUid = "{663E77A6-E25A-4665-BDB1-96FD9EA3946B}";
static const QString CorrectKognErrorUid = "{E3A50002-2785-4EC0-A7B5-2E1D20899C26}";
}

///< Рассчитанные показатели для этапа
struct FactorValues
{
    double latent;
    double swingTime;
    double swingAmpl;
    double swingSpeed;
    double spurtTime;
    double spurtAmpl;
    double spurtSpeed;
    double spurtSpeedMM;
    double statism;
    double reactionTime;
    double overshootAmpl;
    double stabilityDeviation;
    double retentionDeviation;
    double processKind;
    double correctMotorTime;
    double correctMotorAmpl;
    double correctMotorPower;
    double correctMotorError;
    double correctKognTime;
    double correctKognAmpl;
    double correctKognPower;
    double correctKognError;
};

}

class StepOffsetResultData;

/*!
 * \brief Класс показателей теста "Ступени" StepOffsetFactors class
 */
class StepOffsetFactors : public ProbeMultifactor
{
public:
    explicit StepOffsetFactors(const QString &testUid,
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

    int stageTime() const;
    int freq() const;
    int diap() const;
    BaseUtils::Directions direction() const;
    int force() const;

    int bufferCompensationCount() const {return m_bufferComp.size();}
    double bufferCompensationValue(const int i) const {return m_bufferComp.at(i);}
    int bufferReturnCount() const {return m_bufferRet.size();}
    double bufferReturnValue(const int i) const {return m_bufferRet.at(i);}

private:
    /*!
     * \brief Получение меток ступеней
     */
    void getStepsLablels();

    /*!
     * \brief Заполнение буферов
     */
    void fillBuffers();

    void readSignal(QList<QList<SignalsDefines::StabRec>> &bufComp,
                    QList<QList<SignalsDefines::StabRec>> &bufRet);

    void averaging(QList<QList<SignalsDefines::StabRec>> &buffers,
                   QList<double> &buffer);

    StepOffsetFactorsDefines::FactorValues m_fctComp;
    StepOffsetFactorsDefines::FactorValues m_fctRet;
    StepOffsetResultData *m_sordata;

    ///< Буфера сигнала
    QList<double> m_bufferComp;
    QList<double> m_bufferRet;

    struct StepRec
    {
        int counterTo;
        int counterFrom;
    };
    QList<StepRec> m_steps;

};

#endif // STEPOFFSETFACTORS_H
