#include "stepoffsetfactors.h"

#include <qmath.h>
#include <QDebug>

#include "baseutils.h"
#include "aanalyserapplication.h"
#include "datadefines.h"
#include "stepoffsetdefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "stabilogram.h"
#include "stepoffsetresultdata.h"

StepOffsetFactors::StepOffsetFactors(const QString &testUid,
                                     const QString &probeUid,
                                     QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

bool StepOffsetFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool StepOffsetFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanStepOffsetResult);
    return de && se;
}

void StepOffsetFactors::calculate()
{
    getStepsLablels();
    fillBuffers();

    calculateFactors(m_bufferComp, m_fctComp);
    calculateFactors(m_bufferRet, m_fctRet);
    calculateTrancientKind(m_fctComp);
    calculateTrancientKind(m_fctRet);

    calculateCorrectionFactors(m_buffersSeparateComp, m_correctsComp, m_fctComp);
    calculateCorrectionFactors(m_buffersSeparateRet, m_correctsRet, m_fctRet);

    addFactor(StepOffsetFactorsDefines::Compensation::LatentUid, m_fctComp.latent);
    addFactor(StepOffsetFactorsDefines::Compensation::SwingTimeUid, m_fctComp.swingTime);
    addFactor(StepOffsetFactorsDefines::Compensation::SwingAmplUid, m_fctComp.swingAmpl);
    addFactor(StepOffsetFactorsDefines::Compensation::SwingSpeedUid, m_fctComp.swingSpeed);
    addFactor(StepOffsetFactorsDefines::Compensation::SpurtTimeUid, m_fctComp.spurtTime);
    addFactor(StepOffsetFactorsDefines::Compensation::SpurtAmplUid, m_fctComp.spurtAmpl);
    addFactor(StepOffsetFactorsDefines::Compensation::SpurtSpeedUid, m_fctComp.spurtSpeed);
    addFactor(StepOffsetFactorsDefines::Compensation::SpurtSpeedMMUid, m_fctComp.spurtSpeedMM);
    addFactor(StepOffsetFactorsDefines::Compensation::StatismUid, m_fctComp.statism);
    addFactor(StepOffsetFactorsDefines::Compensation::ReactionTimeUid, m_fctComp.reactionTime);
    addFactor(StepOffsetFactorsDefines::Compensation::OvershootAmplUid, m_fctComp.overshootAmpl);
    addFactor(StepOffsetFactorsDefines::Compensation::StabilityDeviationUid, m_fctComp.stabilityDeviation);
    addFactor(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid, m_fctComp.retentionDeviation);
    addFactor(StepOffsetFactorsDefines::Compensation::ProcessKindUid, m_fctComp.processKind);
    addFactor(StepOffsetFactorsDefines::Compensation::CorrectMotorTimeUid, m_fctComp.correctMotorTime);
    addFactor(StepOffsetFactorsDefines::Compensation::CorrectMotorAmplUid, m_fctComp.correctMotorAmpl);
    addFactor(StepOffsetFactorsDefines::Compensation::CorrectMotorPowerUid, m_fctComp.correctMotorPower);
    addFactor(StepOffsetFactorsDefines::Compensation::CorrectMotorErrorUid, m_fctComp.correctMotorError);
    addFactor(StepOffsetFactorsDefines::Compensation::CorrectKognTimeUid, m_fctComp.correctKognTime);
    addFactor(StepOffsetFactorsDefines::Compensation::CorrectKognAmplUid, m_fctComp.correctKognAmpl);
    addFactor(StepOffsetFactorsDefines::Compensation::CorrectKognPowerUid, m_fctComp.correctKognPower);
    addFactor(StepOffsetFactorsDefines::Compensation::CorrectKognErrorUid, m_fctComp.correctKognError);
    addFactor(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid, m_fctComp.correctDominance);

    addFactor(StepOffsetFactorsDefines::Return::LatentUid, m_fctRet.latent);
    addFactor(StepOffsetFactorsDefines::Return::SwingTimeUid, m_fctRet.swingTime);
    addFactor(StepOffsetFactorsDefines::Return::SwingAmplUid, m_fctRet.swingAmpl);
    addFactor(StepOffsetFactorsDefines::Return::SwingSpeedUid, m_fctRet.swingSpeed);
    addFactor(StepOffsetFactorsDefines::Return::SpurtTimeUid, m_fctRet.spurtTime);
    addFactor(StepOffsetFactorsDefines::Return::SpurtAmplUid, m_fctRet.spurtAmpl);
    addFactor(StepOffsetFactorsDefines::Return::SpurtSpeedUid, m_fctRet.spurtSpeed);
    addFactor(StepOffsetFactorsDefines::Return::SpurtSpeedMMUid, m_fctRet.spurtSpeedMM);
    addFactor(StepOffsetFactorsDefines::Return::StatismUid, m_fctRet.statism);
    addFactor(StepOffsetFactorsDefines::Return::ReactionTimeUid, m_fctRet.reactionTime);
    addFactor(StepOffsetFactorsDefines::Return::OvershootAmplUid, m_fctRet.overshootAmpl);
    addFactor(StepOffsetFactorsDefines::Return::StabilityDeviationUid, m_fctRet.stabilityDeviation);
    addFactor(StepOffsetFactorsDefines::Return::RetentionDeviationUid, m_fctRet.retentionDeviation);
    addFactor(StepOffsetFactorsDefines::Return::ProcessKindUid, m_fctRet.processKind);
    addFactor(StepOffsetFactorsDefines::Return::CorrectMotorTimeUid, m_fctRet.correctMotorTime);
    addFactor(StepOffsetFactorsDefines::Return::CorrectMotorAmplUid, m_fctRet.correctMotorAmpl);
    addFactor(StepOffsetFactorsDefines::Return::CorrectMotorPowerUid, m_fctRet.correctMotorPower);
    addFactor(StepOffsetFactorsDefines::Return::CorrectMotorErrorUid, m_fctRet.correctMotorError);
    addFactor(StepOffsetFactorsDefines::Return::CorrectKognTimeUid, m_fctRet.correctKognTime);
    addFactor(StepOffsetFactorsDefines::Return::CorrectKognAmplUid, m_fctRet.correctKognAmpl);
    addFactor(StepOffsetFactorsDefines::Return::CorrectKognPowerUid, m_fctRet.correctKognPower);
    addFactor(StepOffsetFactorsDefines::Return::CorrectKognErrorUid, m_fctRet.correctKognError);
    addFactor(StepOffsetFactorsDefines::Return::CorrectDominanceUid, m_fctRet.correctDominance);
}

void StepOffsetFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(StepOffsetFactorsDefines::GroupUid, StepOffsetFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::LatentUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Латентный период"), tr("KompLatent"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::SwingTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Время размаха"), tr("KompSwingT"), tr("сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::SwingAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Амплитуда размаха"), tr("KompSwingA"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::SwingSpeedUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Скорость размаха"), tr("KompSwingV"), tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::SpurtTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Время броска"), tr("KompSpurtT"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::SpurtAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Амплитуда броска"), tr("KompSpurtA"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::SpurtSpeedUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Скорость броска"), tr("KompSpurtV"), tr("%/сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::SpurtSpeedMMUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Скорость броска"), tr("KompSpurtV"), tr("мм/сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::StatismUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Статизм"), tr("KompStatism"), tr("%"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::ReactionTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Время реакции"), tr("KompReactT"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::OvershootAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Амплитуда перерегулирования"), tr("KompOvershoot"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::StabilityDeviationUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Разброс на этапе стабилизации"), tr("KompStabD"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Разброс на этапе удержания"), tr("KompRetentD"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::ProcessKindUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Тип переходного процесса"), tr("KompKind"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::CorrectMotorTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя длительность моторных коррекций"), tr("KompCorrMT"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::CorrectMotorAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя амплитуда моторных коррекций"), tr("KompCorrMA"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::CorrectMotorPowerUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Мощность моторных коррекций"), tr("KompCorrMP"), tr("мм*сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::CorrectMotorErrorUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя ошибка траектории моторных коррекций"), tr("KompCorrME"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::CorrectKognTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя длительность когнитивных коррекций"), tr("KompCorrKT"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::CorrectKognAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя амплитуда когнитивных коррекций"), tr("KompCorrKA"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::CorrectKognPowerUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Мощность когнитивных коррекций"), tr("KompCorrKP"), tr("мм*сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::CorrectKognErrorUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя ошибка траектории когнитивных коррекций"), tr("KompCorrKE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Преобладание коррекций"), tr("CorrDomin"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::LatentUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Латентный период"), tr("RetLatent"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::SwingTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Время размаха"), tr("RetSwingT"), tr("сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::SwingAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Амплитуда размаха"), tr("RetSwingA"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::SwingSpeedUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Скорость размаха"), tr("RetSwingV"), tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::SpurtTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Время броска"), tr("RetSpurtT"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::SpurtAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Амплитуда броска"), tr("RetSpurtA"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::SpurtSpeedUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Скорость броска"), tr("RetSpurtV"), tr("%/сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::SpurtSpeedMMUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Скорость броска"), tr("RetSpurtV"), tr("мм/сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::StatismUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Статизм"), tr("RetStatism"), tr("%"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::ReactionTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Время реакции"), tr("RetReactT"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::OvershootAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Амплитуда перерегулирования"), tr("RetOvershoot"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::StabilityDeviationUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Разброс на этапе стабилизации"), tr("RetStabD"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::RetentionDeviationUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Разброс на этапе удержания"), tr("RetRetentD"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::ProcessKindUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Тип переходного процесса"), tr("RetKind"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::CorrectMotorTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя длительность моторных коррекций"), tr("RetCorrMT"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::CorrectMotorAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя амплитуда моторных коррекций"), tr("RetCorrMA"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::CorrectMotorPowerUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Мощность моторных коррекций"), tr("RetCorrMP"), tr("мм*сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::CorrectMotorErrorUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя ошибка траектории моторных коррекций"), tr("RetCorrME"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::CorrectKognTimeUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя длительность когнитивных коррекций"), tr("RetCorrKT"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::CorrectKognAmplUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя амплитуда когнитивных коррекций"), tr("RetCorrKA"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::CorrectKognPowerUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Мощность когнитивных коррекций"), tr("RetCorrKP"), tr("мм*сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::CorrectKognErrorUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Средняя ошибка траектории когнитивных коррекций"), tr("RetCorrKE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(StepOffsetFactorsDefines::Return::CorrectDominanceUid, StepOffsetFactorsDefines::GroupUid,
                           tr("Преобладание коррекций"), tr("CorrDomin"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
}

int StepOffsetFactors::stageTime() const
{
    if (m_sordata)
        return m_sordata->stageTime();
    return 0;
}

int StepOffsetFactors::freq() const
{
    if (m_sordata)
        return m_sordata->freq();
    return 0;
}

int StepOffsetFactors::diap() const
{
    if (m_sordata)
        return m_sordata->diap();
    return 0;
}

BaseDefines::Directions StepOffsetFactors::direction() const
{
    if (m_sordata)
        return static_cast<BaseDefines::Directions>(m_sordata->direction());
    return BaseDefines::dirUp;
}

int StepOffsetFactors::force() const
{
    if (m_sordata)
        return m_sordata->force();
    return 0;
}


void StepOffsetFactors::getStepsLablels()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStepOffsetResult, baData))
    {
        m_sordata = new StepOffsetResultData(baData);

        for (int i = 0; i < m_sordata->stepsCount(); ++i)
        {
            StepRec step;
            m_sordata->step(i, step.counterTo, step.counterFrom);
            m_steps.append(step);
        }
    }
}

void StepOffsetFactors::fillBuffers()
{
    QList<QList<SignalsDefines::StabRec>> buffersComp;
    QList<QList<SignalsDefines::StabRec>> buffersRet;
    readSignal(buffersComp, buffersRet);
    averaging(buffersComp, m_buffersSeparateComp, m_bufferComp, false);
    averaging(buffersRet, m_buffersSeparateRet, m_bufferRet, true);
}

void StepOffsetFactors::readSignal(QList<QList<SignalsDefines::StabRec> > &bufComp,
                                   QList<QList<SignalsDefines::StabRec> > &bufRet)
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);

        //! По ступеням
        for (int si = 0; si < m_steps.size(); ++si)
        {
            //! Значения границ ступени
            int counterTo = m_steps.at(si).counterTo;
            int counterFrom = m_steps.at(si).counterFrom;
            //! Конец этапа возврата
            int counterToN = stab.size() - 1;   //! Для последнего - конец сигнала
            if (si < m_steps.size() - 1)        //! Для предыдущих - начала следующей ступени
                counterToN = m_steps.at(si + 1).counterTo;

            //! Контроль границ с сигналом
            if (counterTo >= 0 && counterTo < stab.size() &&
                counterFrom >= 0 && counterFrom < stab.size() &&
                counterToN >= 0 && counterToN < stab.size())
            {
                //! Компенсация воздействия
                QList<SignalsDefines::StabRec> buf;
                for (int i = counterTo; i < counterFrom; ++i)
                {
                    SignalsDefines::StabRec rec = stab.value(i);
                    buf.append(rec);
                }
                bufComp.append(buf);

                //! Возврат в исходное состояние
                buf.clear();
                for (int i = counterFrom; i < counterToN; ++i)
                {
                    SignalsDefines::StabRec rec = stab.value(i);
                    buf.append(rec);
                }
                bufRet.append(buf);
            }
        }
    }
}

void StepOffsetFactors::averaging(QList<QList<SignalsDefines::StabRec>> &buffers,
                                  QList<QList<double>> &buffersSeparate,
                                  QVector<double> &buffer,
                                  bool isInverce)
{
    buffersSeparate.clear();
    buffer.clear();

    //! Так как с индексами не очень удобно, то сначала минимальный размер
    int size = INT_MAX;
    for (int i = 0; i < buffers.size(); ++i)
    {
        if (buffers[i].size() < size)
            size = buffers[i].size();
        buffersSeparate.append(QList<double>());
    }

    //! А затем расчет среднего для каждого отсчета
    for (int i = 0; i < size; ++i)
    {
        double average = 0;
        for (int j = 0; j < buffers.size(); ++j)
        {
            double val = buffers[j][i].x;
            if (m_sordata->direction() == BaseDefines::dirUp || m_sordata->direction() == BaseDefines::dirDown)
                val = buffers[j][i].y;
            val = val * 100.0 / m_sordata->force();
            if (isInverce)
                val = 100 - val;
            buffersSeparate[j].append(val);
            average = average + val;
        }
        average = average / buffers.size();
        buffer.append(average);
    }

    BaseUtils::filterLowFreq(buffer, m_sordata->freq(), 3, BaseUtils::fkChebyshev, 0, buffer.size() - 1);
}

double getSpeed(const QVector<double> buffer, const int idx)
{
    if ((idx > 0) && (idx < buffer.size()))
        return buffer[idx] - buffer[idx - 1];
    else
    if (idx == 0 and buffer.size() > 1)
        return buffer[1] - buffer[0];
    else
        return 0;
}

void StepOffsetFactors::calculateFactors(const QVector<double> buffer, StepOffsetFactorsDefines::FactorValues &factors)
{
    //! Возврат, если длина буфера меньше, чем StepOffsetDefines::MinLengthTransient секунды
    if (buffer.size() < m_sordata->freq() * StepOffsetDefines::MinLengthTransient)
        return;

    //! ********************* Латентный период
    int ln = static_cast<int>(m_sordata->freq() * StepOffsetDefines::StartFindTimeLatent);
    double mo = 0;
    for (int i = 0; i < ln; ++i)
        mo = mo + buffer[i];
    mo = mo / ln;
    int i = 0;
    while ((i < buffer.size()) && (fabs(buffer[i] - mo) < StepOffsetDefines::DeltaLatenet))
        ++i;
    factors.latent = static_cast<double>(i) / static_cast<double>(m_sordata->freq());

    int iLat = i;  ///< Запомним позицию латентного периода

    //! ********************* Размах
    double q = mo;
    int j = buffer.size() + 1;
//    double val;
//    if (i < buffer.size())
//        val = buffer[i];
//    else
//        val = buffer[buffer.size() - 1];
    while (i <= buffer.size() - 1) //! Найти глобальный минимум, меньший MO
    {
        if (buffer[i] <= q)
        {
            q = buffer[i];  //! q - минимум, j - время минимума
            j = i;
        };
        ++i;
    }
    i = j;
    if (i < buffer.size() - 1)           //! Установить i
      while ((i <= buffer.size() - 1) && ( buffer[i] < mo ))
        ++i;

    if (i < buffer.size() - 1)       //! Есть размах
    {
        factors.swingAmpl = q - mo;
        factors.swingTime = static_cast<double>(j) / static_cast<double>(m_sordata->freq());
    }
    else                            //! Нет размаха
    {
        factors.swingAmpl = 0.0;
        factors.swingTime = 0.0;
    }
    if (factors.swingTime > 0) //! Скорость размаха
        factors.swingSpeed = fabs(factors.swingAmpl / factors.swingTime);
    else
        factors.swingSpeed = 0;

    //! ********************* Бросок
    double aMin = q;
    double tMin = factors.swingTime;
    if (fabs(factors.swingTime - 0) < 1e-10)  ///< Равносильно   if (factors.swingTime == 0)
    {
      aMin = mo;
      tMin = factors.latent;
      i = iLat;
    }
    q = 0;
    while ((i <= buffer.size() - 1) and (getSpeed(buffer, i) > StepOffsetDefines::Delta0Speed))
    {
      q = q + getSpeed(buffer, i);
      ++i;
    }
    if (i > buffer.size() - 1) i = buffer.size() - 1;
    double aMax = buffer[i];
    double tMax = i;
    factors.spurtAmpl = aMax - aMin;     //! Амплитуда
    factors.spurtTime = (tMax / static_cast<double>(m_sordata->freq()) - tMin); //! Время
    if (fabs(tMax / static_cast<double>(m_sordata->freq()) - tMin) > 0.001)                  //! Макс. скорость
        factors.spurtSpeed = q / (tMax / static_cast<double>(m_sordata->freq()) - tMin);
    else
        factors.spurtSpeed = 0;
    factors.spurtSpeedMM = factors.spurtSpeed / 100.0 * m_sordata->diap() * static_cast<double>(m_sordata->force()) / 100.0;

    //! ********************* Статизм
    mo = 0;
    q = 0;
    for (i = buffer.size() - 1; i > buffer.size() - 2 * m_sordata->freq() - 1; --i)   //! Мат.ожидание последних двух секунд
    {
        if (i < 0) break;
        mo = mo + buffer[i];
    }
    mo = mo / (2 * m_sordata->freq());
    for (i = buffer.size() - 1; i > buffer.size() - 2 * m_sordata->freq() - 1; --i)  //! СКО последних двух секунд
    {
        if (i < 0) break;
        q = q + qPow(buffer[i] - mo, 2);
    }
    q = qSqrt(q / (2 * m_sordata->freq()));
    q = StepOffsetDefines::DeltaReady * q; //! Коридор допуска
    factors.q = q;
    //TODO: ????? QReadyK:=Round(Q);
    factors.statism = mo - 100;

    //! ********************* Время реакции
    i = buffer.size() - 2 * m_sordata->freq() - 1;
    if (i < 0) i = 0;
    while ((i >= 1) && (fabs(mo - buffer[i]) < q))
        --i;
    if (i > 1)
        factors.reactionTime = static_cast<double>(i) / static_cast<double>(m_sordata->freq());
    else
        factors.reactionTime = 0;
    //! Время реакции не может быть меньше, чем время размаха + время броска
    if (factors.reactionTime < factors.swingTime + factors.spurtTime)
        factors.reactionTime = factors.swingTime + factors.spurtTime;

    //! ********************* Ампл. перерег.
    aMax = 0;
    for (int i = 0; i < buffer.size(); ++i)
    {
        //val = buffer[i];
        if ((buffer[i] > aMax) and (buffer[i] > (mo + q)))
            aMax = buffer[i];
    }
    factors.overshootAmpl = aMax;

    //! ********************* СКО стабилизации
    mo = 0;
    int b = static_cast<int>((factors.latent + factors.swingTime + factors.spurtTime) * m_sordata->freq());
    int e = static_cast<int>(factors.reactionTime * m_sordata->freq()) - 1;
    for (int i = b; i <= e; ++i)
        mo = mo + buffer[i];
    if (e - b > 1)
    {
        mo = mo / (e - b);
        for (int i = b; i <= e; ++i)
            q = q + qPow(buffer[i] - mo, 2) / (e - b - 1);
        q = qSqrt(q);
    }
    else
        q = 0;
    factors.stabilityDeviation = q / 100 * static_cast<double>(m_sordata->diap() * m_sordata->force()) / 100;

    //! ********************* СКО удержания
    b = static_cast<int>(factors.reactionTime * m_sordata->freq());
    e = buffer.size() - 1;
    for (int i = b; i <= e; ++i)
        mo = mo + buffer[i];
    if (e - b > 1)
    {
        mo = mo / (e - b);
        for (int i = b; i <= e; ++i)
            q = q + qPow(buffer[i] - mo, 2) / (e - b - 1);
        q = qSqrt(q);
    }
    factors.retentionDeviation = q / 100 * static_cast<double>(m_sordata->diap() * m_sordata->force()) / 100;
}

void StepOffsetFactors::calculateTrancientKind(StepOffsetFactorsDefines::FactorValues &factors) const
{
    double min = factors.latent;
    if (factors.swingAmpl < 0)
        min = factors.swingTime;

    if (factors.spurtAmpl + factors.swingAmpl > 110 &&
//        factors.overshootAmpl > 110 &&
        factors.spurtSpeed > 200)
        factors.processKind = 1;
    else
    if (factors.spurtAmpl + factors.swingAmpl > 110 &&
//        factors.overshootAmpl > 110 &&
        factors.spurtSpeed < 200)
        factors.processKind = 2;
    else
    if (min + factors.spurtTime < 2.5 &&
        factors.spurtAmpl + factors.swingAmpl > 75 &&
        factors.spurtAmpl + factors.swingAmpl <= 110 and
        factors.spurtSpeed >= 150)
        factors.processKind = 3;
    else
    if (//(min + factors.spurtTime < 2.5) &&
        factors.spurtAmpl + factors.swingAmpl > 75 &&
        factors.spurtAmpl + factors.swingAmpl <= 110 &&
        factors.spurtSpeed >= 150)
        factors.processKind = 4;
    else
    if (factors.reactionTime - factors.spurtTime - min < 3)
        factors.processKind = 5;
    else
        factors.processKind = 6;
}

void StepOffsetFactors::calculateCorrectionFactors(QList<QList<double>> buffersSeparate,
                                                   QList<StepOffsetFactorsDefines::CorrectValue> &corrects,
                                                   StepOffsetFactorsDefines::FactorValues &factors)
{
    corrects.clear();
    factors.correctKognCount = 0.0;
    factors.correctKognTime = 0.0;
    factors.correctKognAmpl = 0.0;
    factors.correctKognPower = 0.0;
    factors.correctKognError = 0.0;
    factors.correctMotorCount = 0.0;
    factors.correctMotorTime = 0.0;
    factors.correctMotorAmpl = 0.0;
    factors.correctMotorPower = 0.0;
    factors.correctMotorError = 0.0;

    for (int ib = 0; ib < buffersSeparate.size(); ++ib)
    {
        int dir = 0;
        int oi = -1;
        double ov = 0.0;
        //! От времени размаха до конца участка
        for (int i = static_cast<int>(factors.swingTime * m_sordata->freq()); i < buffersSeparate[ib].size(); ++i)
        {
            if (i > 0)
            {
              //! Начали движение вниз
              if ((dir == 1) && (buffersSeparate[ib][i] - buffersSeparate[ib][i - 1] < 0))
              {
                  if (oi > -1)
                      addExtremum(corrects, factors, i, oi, buffersSeparate[ib][i], ov);
                  oi = i;
                  ov = buffersSeparate[ib][i];
              }

              //! Начали движение вверх
              if ((dir == -1) && (buffersSeparate[ib][i] - buffersSeparate[ib][i - 1] > 0))
              {
                  if (oi > -1)
                      addExtremum(corrects, factors, i, oi, buffersSeparate[ib][i], ov);
                  oi = i;
                  ov = buffersSeparate[ib][i];
              }

              //! Запомнить направление
              if (buffersSeparate[ib][i] - buffersSeparate[ib][i - 1] > 0)
                  dir = 1;
              else
              if (buffersSeparate[ib][i] - buffersSeparate[ib][i - 1] < 0)
                  dir = -1;
            }
        }
    }

    //! Окончательный расчет
    if (factors.correctKognCount > 0)
    {
      factors.correctKognTime = factors.correctKognTime / factors.correctKognCount;
      factors.correctKognAmpl = factors.correctKognAmpl / factors.correctKognCount;
      factors.correctKognPower = (factors.correctKognTime - StepOffsetDefines::KognCorrectTimeBoundLo) *
                                 factors.correctKognAmpl * factors.correctKognCount;
      factors.correctKognError = factors.correctKognError / factors.correctKognCount;
    }
    if (factors.correctMotorCount > 0)
    {
      factors.correctMotorTime = factors.correctMotorTime / factors.correctMotorCount;
      factors.correctMotorAmpl = factors.correctMotorAmpl / factors.correctMotorCount;
      factors.correctMotorPower = (factors.correctMotorTime - StepOffsetDefines::MotorCorrectTimeBoundLo) *
                                 factors.correctMotorAmpl * factors.correctMotorCount;
      factors.correctMotorError = factors.correctMotorError / factors.correctMotorCount;
    }

    //! Доминирование типа коррекций
    if (factors.correctKognPower + factors.correctMotorPower > 0)
        factors.correctDominance = (factors.correctKognPower - factors.correctMotorPower) /
                                   (factors.correctKognPower + factors.correctMotorPower) * 100;
    else
        factors.correctDominance = 0;
}

void StepOffsetFactors::addExtremum(QList<StepOffsetFactorsDefines::CorrectValue> &corrects,
                                    StepOffsetFactorsDefines::FactorValues &factors,
                                    const int i, const int oi, const double v, const double ov)
{
    StepOffsetFactorsDefines::CorrectValue cv;
    cv.time = static_cast<double>(i - oi) / static_cast<double>(m_sordata->freq());
    cv.ampl = fabs(v - ov);
    corrects.append(cv);

    if ((cv.time >= StepOffsetDefines::MotorCorrectTimeBoundLo) && (cv.time <= StepOffsetDefines::MotorCorrectTimeBoundHi))
    {
        factors.correctMotorTime = factors.correctMotorTime + cv.time;
        factors.correctMotorAmpl = factors.correctMotorAmpl + cv.ampl;
        factors.correctMotorError = factors.correctMotorError + fabs(100.0 - v);
        ++factors.correctMotorCount;
    }
    else
    if ((cv.time >= StepOffsetDefines::KognCorrectTimeBoundLo) && (cv.time <= StepOffsetDefines::KognCorrectTimeBoundHi))
    {
        factors.correctKognTime = factors.correctKognTime + cv.time;
        factors.correctKognAmpl = factors.correctKognAmpl + cv.ampl;
        factors.correctKognError = factors.correctKognError + fabs(100.0 - v);
        ++factors.correctKognCount;
    }
}


