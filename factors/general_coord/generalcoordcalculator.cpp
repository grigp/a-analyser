#include "generalcoordcalculator.h"

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
                           QCoreApplication::tr("Дифференц."), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 2
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::RitmUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Ритм"),
                           QCoreApplication::tr("Ритм"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::RitmStabUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Стабильность ритма"),
                           QCoreApplication::tr("Стаб.ритм"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 3
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::FirstStepUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Порог чувствительности"),
                           QCoreApplication::tr("Чувств."), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::DAPercentUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Опережение маркера цели"),
                           QCoreApplication::tr("Опереж.цели"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::LatentUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Латентный период"),
                           QCoreApplication::tr("Лат.период"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::LatentMovingUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Время начала движения после появления сигнала"),
                           QCoreApplication::tr("Время НД"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 4
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::SprAUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда броска"),
                           QCoreApplication::tr("Ампл."), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidPosErrAnlUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Изменение позиции треугольника"),
                           QCoreApplication::tr("Изм.поз"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 5
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::EvlErrXUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка по фронтали"),
                           QCoreApplication::tr("Ошибка X"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::EvlErrYUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка по сагиттали"),
                           QCoreApplication::tr("Ошибка Y"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidSquareErrTstUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка площади (обучение)"),
                           QCoreApplication::tr("Ошибка S"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 6
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::SprA1Uid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда броска"),
                           QCoreApplication::tr("Ампл."), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidAmplErrTstUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Среднее изменение амплитуды тр-ка (обучение)"),
                           QCoreApplication::tr("Изм.ампл"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 7
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidAccMidXUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность позиции (фронталь)"),
                           QCoreApplication::tr("Точность X"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::MidAccMidYUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность позиции (сагитталь)"),
                           QCoreApplication::tr("Точность Y"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 8
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::AccRepeatUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность повторения"),
                           QCoreApplication::tr("Точн.повт"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordCalculatorDefines::AccFormUid, GeneralCoordCalculatorDefines::GroupUid,
                           QCoreApplication::tr("Точность формы"),
                           QCoreApplication::tr("Точн.формы"), QCoreApplication::tr("%"), 0, 2, FactorsDefines::nsDual, 12);
}

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
    addFct(GeneralCoordCalculatorDefines::DifferenceUid, m_valDifference);
    //! Для 2
    addFct(GeneralCoordCalculatorDefines::RitmUid, m_valRitm);
    addFct(GeneralCoordCalculatorDefines::RitmStabUid, m_valRitmStab);
    //! Для 3
    addFct(GeneralCoordCalculatorDefines::FirstStepUid, m_valFirstStep);
    addFct(GeneralCoordCalculatorDefines::DAPercentUid, m_valDAPercent);
    addFct(GeneralCoordCalculatorDefines::LatentUid, m_valLatent);
    addFct(GeneralCoordCalculatorDefines::LatentMovingUid, m_valLatentMoving);
    //! Для 4
    addFct(GeneralCoordCalculatorDefines::SprAUid, m_valSprA);
    addFct(GeneralCoordCalculatorDefines::MidPosErrAnlUid, m_valMidPosErrAnl);
    //! Для 5
    addFct(GeneralCoordCalculatorDefines::EvlErrXUid, m_valEvlErrX);
    addFct(GeneralCoordCalculatorDefines::EvlErrYUid, m_valEvlErrY);
    addFct(GeneralCoordCalculatorDefines::MidSquareErrTstUid, m_valMidSquareErrTst);
    //! Для 6
    addFct(GeneralCoordCalculatorDefines::SprA1Uid, m_valSprA1);
    addFct(GeneralCoordCalculatorDefines::MidAmplErrTstUid, m_valMidAmplErrTst);
    //! Для 7
    addFct(GeneralCoordCalculatorDefines::MidAccMidXUid, m_valMidAccMidX);
    addFct(GeneralCoordCalculatorDefines::MidAccMidYUid, m_valMidAccMidY);
    //! Для 8
    addFct(GeneralCoordCalculatorDefines::AccRepeatUid, m_valAccRepeat);
    addFct(GeneralCoordCalculatorDefines::AccFormUid, m_valAccForm);
}

void GeneralCoordCalculator::addFct(const QString &uid, const double value)
{
    Q_UNUSED(uid);
    Q_UNUSED(value);
}

