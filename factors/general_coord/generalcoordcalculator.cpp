#include "generalcoordcalculator.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "evolventafactors.h"
#include "stepdeviationfactors.h"
#include "stepoffsetfactors.h"
#include "trianglefactors.h"


GeneralCoordCalculator::GeneralCoordCalculator()
{

}

GeneralCoordCalculator::~GeneralCoordCalculator()
{

}

void GeneralCoordCalculator::doCalculate()
{
    if (m_fgEvl && m_fgSO && m_fgSD && m_fgTrngl)
    {
        //! Для 1
        m_valDifference.setValue(m_fgSD->factorValue(StepDeviationFactorsDefines::DifferenceUid));

        //! Для 2
        auto v = m_fgSD->factorValue(StepDeviationFactorsDefines::RitmUid);
        m_valRitm.setValue(v);
        if (v >= 1)
            m_valRitm.setPercent((1 - (v - 1) / (m_valRitm.max - 1)) * 100);
        else
            m_valRitm.setPercent((v - m_valRitm.min) / (1 - m_valRitm.min) * 100);
        m_valRitmStab.setValue(m_fgSD->factorValue(StepDeviationFactorsDefines::RitmStabUid));

        //! Для 3
        m_valFirstStep.setValue(m_fgSD->factorValue(StepDeviationFactorsDefines::SensitivityUid));
        v = m_fgEvl->factorValue(EvolventaFactorsDefines::DAPercent);
        m_valDAPercent.setValue(v);
        if (v >= -20)
            m_valDAPercent.percent = (1 - (v - (-20)) / (m_valDAPercent.max - (-20))) * 100;
        else
            m_valDAPercent.percent = (v - m_valDAPercent.min) / (100 - m_valDAPercent.min) * 100;
        m_valLatent.setValue(m_fgSO->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid));
        m_valLatentMoving.setValue(m_fgTrngl->factorValue(TriangleFactorsDefines::LatentMovingUid));
        //! Для 4
//        m_valSprA.setValue();
//        m_valMidPosErrAnl.setValue();
//        //! Для 5
//        m_valEvlErrX.setValue();
//        m_valEvlErrY.setValue();
//        m_valMidSquareErrTst.setValue();
//        //! Для 6
//        m_valSprA1.setValue();
//        m_valMidAmplErrTst.setValue();
//        //! Для 7
//        m_valMidAccMidX.setValue();
//        m_valMidAccMidY.setValue();
//        //! Для 8
//        m_valAccRepeat.setValue();
//        m_valAccForm.setValue();
    }
}

void GeneralCoordCalculator::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(GeneralCoordCalculatorDefines::GroupUid, GeneralCoordCalculatorDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::GeneralCoordUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Общий показатель координированности"),
                           QCoreApplication::tr("ОПК"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::CapacitySetPosAfterShiftUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Способность оценивать и занимать определенную позу после смещения тела"),
                           QCoreApplication::tr("Уст.позы"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::CapacityRitmMovingUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Способность к ритмичному движению"),
                           QCoreApplication::tr("Ритмичность"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MotionAccuracyBeginUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность вступления в сложное движение"),
                           QCoreApplication::tr("Точн.движ"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::FctFinalAccuracyUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность при финальном требовании"),
                           QCoreApplication::tr("Точн.треб"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::ProcessPerformAccuracyUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность выполнения процесса"),
                           QCoreApplication::tr("Точн.проц"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::AmplitudePerformAccuracyUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность воспроизводства требуемой амплитуды"),
                           QCoreApplication::tr("Точн.ампл"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::OrientInSpaceUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Ориентация в пространстве"),
                           QCoreApplication::tr("Ориентация"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::CapacityRepeatMovingUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Способность воспринимать заданное движение (обучаемость)"),
                           QCoreApplication::tr("Обучаемость"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);

    //! Для 1
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::DifferenceUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Дифференциация"),
                           QCoreApplication::tr("Дифференц."), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    //! Для 2
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::RitmUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Ритм"),
                           QCoreApplication::tr("Ритм"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::RitmStabUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Стабильность ритма"),
                           QCoreApplication::tr("Стаб.ритм"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    //! Для 3
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::FirstStepUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Порог чувствительности"),
                           QCoreApplication::tr("Чувств."), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::DAPercentUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Опережение маркера цели"),
                           QCoreApplication::tr("Опереж.цели"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::LatentUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Латентный период"),
                           QCoreApplication::tr("Лат.период"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::LatentMovingUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Время начала движения после появления сигнала"),
                           QCoreApplication::tr("Время НД"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    //! Для 4
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::SprAUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда броска"),
                           QCoreApplication::tr("Ампл."), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidPosErrAnlUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Изменение позиции треугольника"),
                           QCoreApplication::tr("Изм.поз"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    //! Для 5
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::EvlErrXUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка по фронтали"),
                           QCoreApplication::tr("Ошибка X"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::EvlErrYUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка по сагиттали"),
                           QCoreApplication::tr("Ошибка Y"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidSquareErrTstUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка площади (обучение)"),
                           QCoreApplication::tr("Ошибка S"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    //! Для 6
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::SprA1Uid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда броска"),
                           QCoreApplication::tr("Ампл."), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidAmplErrTstUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Среднее изменение амплитуды тр-ка (обучение)"),
                           QCoreApplication::tr("Изм.ампл"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    //! Для 7
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidAccMidXUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность позиции (фронталь)"),
                           QCoreApplication::tr("Точность X"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidAccMidYUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность позиции (сагитталь)"),
                           QCoreApplication::tr("Точность Y"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    //! Для 8
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::AccRepeatUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность повторения"),
                           QCoreApplication::tr("Точн.повт"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::AccFormUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность формы"),
                           QCoreApplication::tr("Точн.формы"), QCoreApplication::tr("%"), 2, 2, FactorsDefines::nsDual, 12);
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valDifference() const
{
    return m_valDifference;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valRitm() const
{
    return m_valRitm;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valRitmStab() const
{
    return m_valRitmStab;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valFirstStep() const
{
    return m_valFirstStep;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valDAPercent() const
{
    return m_valDAPercent;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valLatent() const
{
    return m_valLatent;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valLatentMoving()
{
    return m_valLatentMoving;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valSprA() const
{
    return m_valSprA;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valMidPosErrAnl() const
{
    return m_valMidPosErrAnl;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valEvlErrX() const
{
    return m_valEvlErrX;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valEvlErrY() const
{
    return m_valEvlErrY;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valMidSquareErrTst() const
{
    return m_valMidSquareErrTst;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valSprA1() const
{
    return m_valSprA1;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valMidAmplErrTst() const
{
    return m_valMidAmplErrTst;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valMidAccMidX() const
{
    return m_valMidAccMidX;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valMidAccMidY()
{
    return m_valMidAccMidY;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valAccRepeat() const
{
    return m_valAccRepeat;
}

GeneralCoordDefines::DiapValue GeneralCoordCalculator::valAccForm() const
{
    return m_valAccForm;
}

//void GeneralCoordCalculator::setValDifference(const double val)
//{
//    m_valDifference.setValue(val);
//}

void GeneralCoordCalculator::addFactors()
{
    addFct(GeneralCoordCalculatorDefines::GeneralCoordUid, m_valGeneralCoord);

    addFct(GeneralCoordCalculatorDefines::CapacitySetPosAfterShiftUid, m_valCapacitySetPosAfterShift);
    addFct(GeneralCoordCalculatorDefines::CapacityRitmMovingUid, m_valCapacityRitmMoving);
    addFct(GeneralCoordCalculatorDefines::MotionAccuracyBeginUid, m_valMotionAccuracyBegin);
    addFct(GeneralCoordCalculatorDefines::FctFinalAccuracyUid, m_valFctFinalAccuracy);
    addFct(GeneralCoordCalculatorDefines::ProcessPerformAccuracyUid, m_valProcessPerformAccuracy);
    addFct(GeneralCoordCalculatorDefines::AmplitudePerformAccuracyUid, m_valAmplitudePerformAccuracy);
    addFct(GeneralCoordCalculatorDefines::OrientInSpaceUid, m_valOrientInSpace);
    addFct(GeneralCoordCalculatorDefines::CapacityRepeatMovingUid, m_valCapacityRepeatMoving);

    //! Для 1
    addFct(GeneralCoordCalculatorDefines::DifferenceUid, m_valDifference.value);
    //! Для 2
    addFct(GeneralCoordCalculatorDefines::RitmUid, m_valRitm.value);
    addFct(GeneralCoordCalculatorDefines::RitmStabUid, m_valRitmStab.value);
    //! Для 3
    addFct(GeneralCoordCalculatorDefines::FirstStepUid, m_valFirstStep.value);
    addFct(GeneralCoordCalculatorDefines::DAPercentUid, m_valDAPercent.value);
    addFct(GeneralCoordCalculatorDefines::LatentUid, m_valLatent.value);
    addFct(GeneralCoordCalculatorDefines::LatentMovingUid, m_valLatentMoving.value);
    //! Для 4
    addFct(GeneralCoordCalculatorDefines::SprAUid, m_valSprA.value);
    addFct(GeneralCoordCalculatorDefines::MidPosErrAnlUid, m_valMidPosErrAnl.value);
    //! Для 5
    addFct(GeneralCoordCalculatorDefines::EvlErrXUid, m_valEvlErrX.value);
    addFct(GeneralCoordCalculatorDefines::EvlErrYUid, m_valEvlErrY.value);
    addFct(GeneralCoordCalculatorDefines::MidSquareErrTstUid, m_valMidSquareErrTst.value);
    //! Для 6
    addFct(GeneralCoordCalculatorDefines::SprA1Uid, m_valSprA1.value);
    addFct(GeneralCoordCalculatorDefines::MidAmplErrTstUid, m_valMidAmplErrTst.value);
    //! Для 7
    addFct(GeneralCoordCalculatorDefines::MidAccMidXUid, m_valMidAccMidX.value);
    addFct(GeneralCoordCalculatorDefines::MidAccMidYUid, m_valMidAccMidY.value);
    //! Для 8
    addFct(GeneralCoordCalculatorDefines::AccRepeatUid, m_valAccRepeat.value);
    addFct(GeneralCoordCalculatorDefines::AccFormUid, m_valAccForm.value);
}

void GeneralCoordCalculator::addFct(const QString &uid, const double value)
{
    Q_UNUSED(uid);
    Q_UNUSED(value);
}

