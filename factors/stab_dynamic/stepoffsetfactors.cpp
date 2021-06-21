#include "stepoffsetfactors.h"

#include <qmath.h>

#include "aanalyserapplication.h"
#include "datadefines.h"
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
}

void StepOffsetFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(StepOffsetFactorsDefines::GroupUid, tr("Показатели теста \"Ступени\""));

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
                           tr("Статизм"), tr("KompStatism"), tr("%"), 1, 3, FactorsDefines::nsDual, 12);
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
                           tr("Статизм"), tr("RetStatism"), tr("%"), 1, 3, FactorsDefines::nsDual, 12);
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
}

void StepOffsetFactors::getStepsLablels()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanCrossResult, baData))
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
    averaging(buffersComp, m_bufferComp);
    averaging(buffersRet, m_bufferRet);
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
            int counterToN = stab.size();   //! Для последнего - конец сигнала
            if (si < m_steps.size() - 1)    //! Для предыдущих - начала следующей ступени
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

void StepOffsetFactors::averaging(QList<QList<SignalsDefines::StabRec> > &buffers,
                                  QList<SignalsDefines::StabRec> &buffer)
{
    //! Так как с индексами не очень удобно, то сначала минимальный размер
    int size = INT_MAX;
    for (int i = 0; i < buffers.size(); ++i)
        if (buffers[i].size() < size)
            size = buffers[i].size();

    //! А затем расчет среднего для каждого отсчета
    for (int i = 0; i < size; ++i)
    {
        SignalsDefines::StabRec rec;
        rec.x = 0;
        rec.y = 0;
        for (int j = 0; j < buffers.size(); ++j)
        {
            rec.x = rec.x + buffers[j][i].x;
            rec.y = rec.y + buffers[j][i].y;
        }
        rec.x = rec.x / buffers.size();
        rec.y = rec.y / buffers.size();

        buffer.append(rec);
    }
}

