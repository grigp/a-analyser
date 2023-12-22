#include "generalcoordfactors.h"

#include "aanalyserapplication.h"

GeneralCoordFactors::GeneralCoordFactors(const QStringList &testUids, QObject *parent)
    : BaseMultifactor (testUids, parent)
{

}

QString GeneralCoordFactors::uid() const
{
    return GeneralCoordFactorsDefines::GroupUid;
}

QString GeneralCoordFactors::name() const
{
    return GeneralCoordFactorsDefines::GroupName;
}

bool GeneralCoordFactors::isValid() const
{
    return isValid(testUids());
}

bool GeneralCoordFactors::isValid(const QStringList &testUids)
{

}

void GeneralCoordFactors::calculate()
{
    addFactor(GeneralCoordFactorsDefines::GeneralCoordUid, m_valGeneralCoord);

    addFactor(GeneralCoordFactorsDefines::CapacitySetPosAfterShiftUid, m_valCapacitySetPosAfterShift);
    addFactor(GeneralCoordFactorsDefines::CapacityRitmMovingUid, m_valCapacityRitmMoving);
    addFactor(GeneralCoordFactorsDefines::MotionAccuracyBeginUid, m_valMotionAccuracyBegin);
    addFactor(GeneralCoordFactorsDefines::FctFinalAccuracyUid, m_valFctFinalAccuracy);
    addFactor(GeneralCoordFactorsDefines::ProcessPerformAccuracyUid, m_valProcessPerformAccuracy);
    addFactor(GeneralCoordFactorsDefines::AmplitudePerformAccuracyUid, m_valAmplitudePerformAccuracy);
    addFactor(GeneralCoordFactorsDefines::OrientInSpaceUid, m_valOrientInSpace);
    addFactor(GeneralCoordFactorsDefines::CapacityRepeatMovingUid, m_valCapacityRepeatMoving);

    //! Для 1
    addFactor(GeneralCoordFactorsDefines::DifferenceUid, m_valDifference);
    //! Для 2
    addFactor(GeneralCoordFactorsDefines::RitmUid, m_valRitm);
    addFactor(GeneralCoordFactorsDefines::RitmStabUid, m_valRitmStab);
    //! Для 3
    addFactor(GeneralCoordFactorsDefines::FirstStepUid, m_valFirstStep);
    addFactor(GeneralCoordFactorsDefines::DAPercentUid, m_valDAPercent);
    addFactor(GeneralCoordFactorsDefines::LatentUid, m_valLatent);
    addFactor(GeneralCoordFactorsDefines::LatentMovingUid, m_valLatentMoving);
    //! Для 4
    addFactor(GeneralCoordFactorsDefines::SprAUid, m_valSprA);
    addFactor(GeneralCoordFactorsDefines::MidPosErrAnlUid, m_valMidPosErrAnl);
    //! Для 5
    addFactor(GeneralCoordFactorsDefines::EvlErrXUid, m_valEvlErrX);
    addFactor(GeneralCoordFactorsDefines::EvlErrYUid, m_valEvlErrY);
    addFactor(GeneralCoordFactorsDefines::MidSquareErrTstUid, m_valMidSquareErrTst);
    //! Для 6
    addFactor(GeneralCoordFactorsDefines::SprA1Uid, m_valSprA1);
    addFactor(GeneralCoordFactorsDefines::MidAmplErrTstUid, m_valMidAmplErrTst);
    //! Для 7
    addFactor(GeneralCoordFactorsDefines::MidAccMidXUid, m_valMidAccMidX);
    addFactor(GeneralCoordFactorsDefines::MidAccMidYUid, m_valMidAccMidY);
    //! Для 8
    addFactor(GeneralCoordFactorsDefines::AccRepeatUid, m_valAccRepeat);
    addFactor(GeneralCoordFactorsDefines::AccFormUid, m_valAccForm);
}

void GeneralCoordFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(GeneralCoordFactorsDefines::GroupUid, GeneralCoordFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::GeneralCoordUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Общий показатель координированности"), tr("ОПК"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::CapacitySetPosAfterShiftUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Способность оценивать и занимать определенную позу после смещения тела"),
                           tr("Уст.позы"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::CapacityRitmMovingUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Способность к ритмичному движению"), tr("Ритмичность"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::MotionAccuracyBeginUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Точность вступления в сложное движение"), tr("Точн.движ"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::FctFinalAccuracyUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Точность при финальном требовании"), tr("Точн.треб"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::ProcessPerformAccuracyUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Точность выполнения процесса"), tr("Точн.проц"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::AmplitudePerformAccuracyUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Точность воспроизводства требуемой амплитуды"), tr("Точн.ампл"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::OrientInSpaceUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Ориентация в пространстве"), tr("Ориентация"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::CapacityRepeatMovingUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Способность воспринимать заданное движение (обучаемость)"), tr("Обучаемость"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);

    //! Для 1
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::DifferenceUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Дифференциация"), tr("Дифференц."), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 2
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::RitmUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Ритм"), tr("Ритм"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::RitmStabUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Стабильность ритма"), tr("Стаб.ритм"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 3
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::FirstStepUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Порог чувствительности"), tr("Чувств."), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::DAPercentUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Опережение маркера цели"), tr("Опереж.цели"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::LatentUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Латентный период"), tr("Лат.период"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::LatentMovingUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Время начала движения после появления сигнала"), tr("Время НД"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 4
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::SprAUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Амплитуда броска"), tr("Ампл."), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::MidPosErrAnlUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Изменение позиции треугольника"), tr("Изм.поз"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 5
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::EvlErrXUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Средняя ошибка по фронтали"), tr("Ошибка X"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::EvlErrYUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Средняя ошибка по сагиттали"), tr("Ошибка Y"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::MidSquareErrTstUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Средняя ошибка площади (обучение)"), tr("Ошибка S"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 6
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::SprA1Uid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Амплитуда броска"), tr("Ампл."), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::MidAmplErrTstUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Среднее изменение амплитуды тр-ка (обучение)"), tr("Изм.ампл"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 7
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::MidAccMidXUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Точность позиции (фронталь)"), tr("Точность X"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::MidAccMidYUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Точность позиции (сагитталь)"), tr("Точность Y"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    //! Для 8
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::AccRepeatUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Точность повторения"), tr("Точн.повт"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(GeneralCoordFactorsDefines::AccFormUid, GeneralCoordFactorsDefines::GroupUid,
                           tr("Точность формы"), tr("Точн.формы"), tr("%"), 0, 2, FactorsDefines::nsDual, 12);
}
