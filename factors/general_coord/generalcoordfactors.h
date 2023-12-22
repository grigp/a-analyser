#ifndef GENERALCOORDFACTORS_H
#define GENERALCOORDFACTORS_H

#include <QObject>

#include "multifactor.h"

namespace GeneralCoordFactorsDefines
{
static const QString GroupUid = "{5730D1B0-CB89-4004-A178-BEC12219F50E}";
static const QString GroupName = QCoreApplication::tr("Показатели оценки координационных способностей");

static const QString GeneralCoordUid = "{6581C206-06C2-4D68-842C-D9EBDEEB1BF8}";

static const QString CapacitySetPosAfterShiftUid = "{B6621B4D-DCCC-4B5A-B086-58D1F6942338}";
static const QString CapacityRitmMovingUid = "{FC36E8F1-3C9C-4767-A7F5-08A423F3E5CF}";
static const QString MotionAccuracyBeginUid = "{AD0BA86B-C0A7-43CB-91DD-6BDD2FE2D495}";
static const QString FctFinalAccuracyUid = "{52C70F63-9EAE-42E3-9F5C-49C68E2A05B5}";
static const QString ProcessPerformAccuracyUid = "{6266C5C7-30B6-4DA3-B400-5C18C826A1E7}";
static const QString AmplitudePerformAccuracyUid = "{41E7989C-0184-43A0-8F3A-AC0A9987498C}";
static const QString OrientInSpaceUid = "{E7A48AC9-6FD4-4E81-A7A6-8567D8A757BF}";
static const QString CapacityRepeatMovingUid = "{6BD138BE-6C83-4566-B938-7CD57E00767D}";

//! Для 1
static const QString DifferenceUid = "{C7BD857A-95E8-416E-B53C-ECA8B7217456}";
//! Для 2
static const QString RitmUid = "{A20A081F-4CC7-41BB-A471-869E3C5A733B}";
static const QString RitmStabUid = "{7C3446EF-0FF3-4610-8FB8-C0F3308BFC4B}";
//! Для 3
static const QString FirstStepUid = "{652F5882-941C-4FF9-9715-F205AFFFC4BF}";
static const QString DAPercentUid = "{DD2FDCB3-1262-4E10-B611-02D6A491F608}";
static const QString LatentUid = "{D2479174-5327-43FD-89C9-0F1DD00BB8E6}";
static const QString LatentMovingUid = "{0804D7BE-AE27-4F37-8C2F-53C19AED7590}";
//! Для 4
static const QString SprAUid = "{E1603F9C-A9EB-4C67-9C16-193BA311DC6C}";
static const QString MidPosErrAnlUid = "{2DD72092-E3D9-4B5A-B6B0-935F1E783305}";
//! Для 5
static const QString EvlErrXUid = "{5DA260CE-778B-4E58-A5FC-CE5110AD2B7B}";
static const QString EvlErrYUid = "{49F62CAF-9B0D-4AF4-B5E5-3DC195C87479}";
static const QString MidSquareErrTstUid = "{CFE547DE-0433-427B-8A37-1227521C19A4}";
//! Для 6
static const QString SprA1Uid = "{8AFE8D6B-38E1-4A3E-B359-F583DDFB9C4A}";
static const QString MidAmplErrTstUid = "{740FFF0B-CFCD-49CA-8850-7BCA279C8CEF}";
//! Для 7
static const QString MidAccMidXUid = "{50A85AD9-158E-4629-AF5D-6EF2E499CA0D}";
static const QString MidAccMidYUid = "{4ED945A1-806C-4ED7-A77C-8298157E421F}";
//! Для 8
static const QString AccRepeatUid = "{59332022-D75C-40FD-B09C-32EF9C4AA25D}";
static const QString AccFormUid = "{0775F924-556F-41FD-845E-2C4AB4110775}";

}

/*!
 * \brief Класс расчета показателей оценки координационных способностей The GeneralCoordFactors class
 * На основе нескольких тестов
 */
class GeneralCoordFactors : public BaseMultifactor
{
    Q_OBJECT
public:
    explicit GeneralCoordFactors(const QStringList &testUids, QObject *parent = nullptr);

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
    static bool isValid(const QStringList &testUids);

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
    double m_valGeneralCoord {0};

    double m_valCapacitySetPosAfterShift {0};
    double m_valCapacityRitmMoving {0};
    double m_valMotionAccuracyBegin {0};
    double m_valFctFinalAccuracy {0};
    double m_valProcessPerformAccuracy {0};
    double m_valAmplitudePerformAccuracy {0};
    double m_valOrientInSpace {0};
    double m_valCapacityRepeatMoving {0};

    //! Для 1
    double m_valDifference {0};
    //! Для 2
    double m_valRitm {0};
    double m_valRitmStab {0};
    //! Для 3
    double m_valFirstStep {0};
    double m_valDAPercent {0};
    double m_valLatent {0};
    double m_valLatentMoving {0};
    //! Для 4
    double m_valSprA {0};
    double m_valMidPosErrAnl {0};
    //! Для 5
    double m_valEvlErrX {0};
    double m_valEvlErrY {0};
    double m_valMidSquareErrTst {0};
    //! Для 6
    double m_valSprA1 {0};
    double m_valMidAmplErrTst {0};
    //! Для 7
    double m_valMidAccMidX {0};
    double m_valMidAccMidY {0};
    //! Для 8
    double m_valAccRepeat {0};
    double m_valAccForm {0};
};

#endif // GENERALCOORDFACTORS_H
