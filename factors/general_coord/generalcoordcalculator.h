#ifndef GENERALCOORDCALCULATOR_H
#define GENERALCOORDCALCULATOR_H

#include <QObject>

#include <QCoreApplication>

namespace GeneralCoordCalculatorDefines
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

class EvolventaFactors;
class StepDeviationFactors;
class StepOffsetFactors;
class TriangleFactors;

/*!
 * \brief Базовый класс, осуществляющий расчет показателей оценки координационных способностей The GeneralCoordCalculator class
 */
class GeneralCoordCalculator
{
public:
    GeneralCoordCalculator();
    virtual ~GeneralCoordCalculator();

    void doCalculate();

    /*!
     * \brief Регистрирует факторсгруппу и показатели
     */
    static void registerFactors();

protected:
    double valGeneralCoord() const {return m_valGeneralCoord;}
    void setValGeneralCoord(const double val) {m_valGeneralCoord = val;}

    double valCapacitySetPosAfterShift() const {return m_valCapacitySetPosAfterShift;}
    void setValCapacitySetPosAfterShift(const double val) {m_valCapacitySetPosAfterShift = val;}

    double valCapacityRitmMoving() const {return m_valCapacityRitmMoving;}
    void setValCapacityRitmMoving(const double val) {m_valCapacityRitmMoving = val;}

    double valMotionAccuracyBegin() const {return m_valMotionAccuracyBegin;}
    void setValMotionAccuracyBegin(const double val) {m_valMotionAccuracyBegin = val;}

    double valFctFinalAccuracy() const {return m_valFctFinalAccuracy;}
    void setValFctFinalAccuracy(const double val) {m_valFctFinalAccuracy = val;}

    double valProcessPerformAccuracy() const {return m_valProcessPerformAccuracy;}
    void setValProcessPerformAccuracy(const double val) {m_valProcessPerformAccuracy = val;}

    double valAmplitudePerformAccuracy() const {return m_valAmplitudePerformAccuracy;}
    void setValAmplitudePerformAccuracy(const double val) {m_valAmplitudePerformAccuracy = val;}

    double valOrientInSpace() const {return m_valOrientInSpace;}
    void setValOrientInSpace(const double val) {m_valOrientInSpace = val;}

    double valCapacityRepeatMoving() const {return m_valCapacityRepeatMoving;}
    void setValCapacityRepeatMoving(const double val) {m_valCapacityRepeatMoving = val;}

    //! Для 1
    double valDifference() const {return m_valDifference;}
    void setValDifference(const double val) {m_valDifference = val;}
    //! Для 2
    double valRitm() const {return m_valRitm;}
    void setValRitm(const double val) {m_valRitm = val;}
    double valRitmStab() const {return m_valRitmStab;}
    void setValRitmStab(const double val) {m_valRitmStab = val;}
    //! Для 3
    double valFirstStep() const {return m_valFirstStep;}
    void setValFirstStep(const double val) {m_valFirstStep = val;}
    double valDAPercent() const {return m_valDAPercent;}
    void setValDAPercent(const double val) {m_valDAPercent = val;}
    double valLatent() const {return m_valLatent;}
    void setValLatent(const double val) {m_valLatent = val;}
    double valLatentMoving() {return m_valLatentMoving;}
    void setValLatentMoving(const double val) {m_valLatentMoving = val;}
    //! Для 4
    double valSprA() const {return m_valSprA;}
    void setValSprA(const double val) {m_valSprA = val;}
    double valMidPosErrAnl() const {return m_valMidPosErrAnl;}
    void setValMidPosErrAnl(const double val) {m_valMidPosErrAnl = val;}
    //! Для 5
    double valEvlErrX() const {return m_valEvlErrX;}
    void setValEvlErrX(const double val) {m_valEvlErrX = val;}
    double valEvlErrY() const {return m_valEvlErrY;}
    void setValEvlErrY(const double val) {m_valEvlErrY = val;}
    double valMidSquareErrTst() const {return m_valMidSquareErrTst;}
    void setValMidSquareErrTst(const double val) {m_valMidSquareErrTst = val;}
    //! Для 6
    double valSprA1() const {return m_valSprA1;}
    void setValSprA1(const double val) {m_valSprA1 = val;}
    double valMidAmplErrTst() const {return m_valMidAmplErrTst;}
    void setValMidAmplErrTst(const double val) {m_valMidAmplErrTst = val;}
    //! Для 7
    double valMidAccMidX() const {return m_valMidAccMidX;}
    void setValMidAccMidX(const double val) {m_valMidAccMidX = val;}
    double valMidAccMidY() {return m_valMidAccMidY;}
    void setValMidAccMidY(const double val) {m_valMidAccMidY = val;}
    //! Для 8
    double valAccRepeat() const {return m_valAccRepeat;}
    void setValAccRepeat(const double val) {m_valAccRepeat = val;}
    double valAccForm() const {return m_valAccForm;}
    void setValAccForm(const double val) {m_valAccForm = val;}

    virtual void addFactors();

    virtual void addFct(const QString &uid, const double value);

    EvolventaFactors *evolventaFactors() const {return m_fgEvl;}
    void setFGEvolventa(EvolventaFactors *fgEvl) {m_fgEvl = fgEvl;}

    StepDeviationFactors *stepDeviationFactors() const {return m_fgSD;}
    void setFGStepDeviation(StepDeviationFactors *fgSD) {m_fgSD = fgSD;}

    StepOffsetFactors *stepOffsetFactors() const {return m_fgSO;}
    void setFGStepOffset(StepOffsetFactors *fgSO) {m_fgSO = fgSO;}

    TriangleFactors *triangleFactors() const {return m_fgTrngl;}
    void setFGTriangle(TriangleFactors *fgTrngl) {m_fgTrngl = fgTrngl;}

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

    EvolventaFactors *m_fgEvl {nullptr};
    StepDeviationFactors *m_fgSD {nullptr};
    StepOffsetFactors *m_fgSO {nullptr};
    TriangleFactors *m_fgTrngl {nullptr};
};

#endif // GENERALCOORDCALCULATOR_H
