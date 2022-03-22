#include "trenresultfactors.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "trenresultdata.h"
#include "takeputresultdata.h"
#include "stabilogram.h"

TrenResultFactors::TrenResultFactors(const QString &testUid,
                                     const QString &probeUid,
                                     const QString &channelId,
                                     QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool TrenResultFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool TrenResultFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctTrenResult;
}

void TrenResultFactors::calculate()
{
    //! Основные показатели тренажеров
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), channelId(), baData))
    {
        TrenResultData data(baData);
        for (int i = 0; i < data.size(); ++i)
        {
            FactorsDefines::FactorValue fv = data.factor(i);
            addFactor(fv.uid(), fv.value());
        }
    }

    //! Показатели захвата - укладки
    baData.clear();
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanTakePutResult, baData))
    {
        getSignal();

        //! Расчет средних длительности и скорости
        int prevCnt = 0;
        double midLen[3];
        double midSpeed[3];
        int cntEvents[3];
        for (int i = 0; i < 3; ++i)
        {
            midLen[i] = 0;
            midSpeed[i] = 0;
            cntEvents[i] = 0;
        }
        TakePutResultData data(baData);
        for (int i = 0; i < data.size(); ++i)
        {
            auto event = data.event(i);
            double len = 0;
            if (event.counter - prevCnt >= 10)
                len = static_cast<double>(event.counter - prevCnt) / m_freqency;
            auto speed = getSpeed(prevCnt, event.counter);

            midLen[event.kind] += len;
            midSpeed[event.kind] += speed;
            ++cntEvents[event.kind];

            prevCnt = event.counter;
        }

        //! Запись показателей
        double valLT = 0;
        double valST = 0;
        if (cntEvents[TakePutResultData::tpkTake] > 0)
        {
            valLT = midLen[TakePutResultData::tpkTake] / cntEvents[TakePutResultData::tpkTake];
            valST = midSpeed[TakePutResultData::tpkTake] / cntEvents[TakePutResultData::tpkTake];
        }

        double valLP = 0;
        double valSP = 0;
        if (cntEvents[TakePutResultData::tpkPut] > 0)
        {
            valLP = midLen[TakePutResultData::tpkPut] / cntEvents[TakePutResultData::tpkPut];
            valSP = midSpeed[TakePutResultData::tpkPut] / cntEvents[TakePutResultData::tpkPut];
        }

        double valLE = 0;
        double valSE = 0;
        if (cntEvents[TakePutResultData::tpkError] > 0)
        {
            valLE = midLen[TakePutResultData::tpkError] / cntEvents[TakePutResultData::tpkError];
            valSE = midSpeed[TakePutResultData::tpkError] / cntEvents[TakePutResultData::tpkError];
        }
        addFactor(TrenResultFactorsDefines::TakeStageTime, valLT);
        addFactor(TrenResultFactorsDefines::PutStageTime, valLP);
        addFactor(TrenResultFactorsDefines::ErrorStageTime, valLE);
        addFactor(TrenResultFactorsDefines::TakeStageSpeed, valST);
        addFactor(TrenResultFactorsDefines::PutStageSpeed, valSP);
        addFactor(TrenResultFactorsDefines::ErrorStageSpeed, valSE);

        m_isTakePutExists = true;
    }
}

void TrenResultFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(TrenResultFactorsDefines::GroupUid, tr("Показатели тренажеров"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::ScoreUid, TrenResultFactorsDefines::GroupUid,
                           tr("Количество набранных очков"), tr("Очки"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::FaultsUid, TrenResultFactorsDefines::GroupUid,
                           tr("Количество ошибок"), tr("Ошибки"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::TimeUid, TrenResultFactorsDefines::GroupUid,
                           tr("Время игры"), tr("Время"), tr("сек"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::RowsDeletedUid, TrenResultFactorsDefines::GroupUid,
                           tr("Количество удаленных строк"), tr("Строки"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::TakeStageTime, TrenResultFactorsDefines::GroupUid,
                           tr("Длительность интервалов захвата"), tr("TakeTime"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::PutStageTime, TrenResultFactorsDefines::GroupUid,
                           tr("Длительность интервалов укладки"), tr("PutTime"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::ErrorStageTime, TrenResultFactorsDefines::GroupUid,
                           tr("Длительность интервалов ошибок"), tr("ErrTime"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::TakeStageSpeed, TrenResultFactorsDefines::GroupUid,
                           tr("Скорость на этапе захвата"), tr("TakeSpd"), tr("мм/сек"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::PutStageSpeed, TrenResultFactorsDefines::GroupUid,
                           tr("Скорость на этапе укладки"), tr("PutSpd"), tr("мм/сек"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TrenResultFactorsDefines::ErrorStageSpeed, TrenResultFactorsDefines::GroupUid,
                           tr("Скорость на этапе ошибок"), tr("ErrSpd"), tr("мм/сек"), 0, 3, FactorsDefines::nsDual, 12);

}

void TrenResultFactors::getSignal()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baData))
    {
        m_signal = new Stabilogram(baData);
        m_freqency = m_signal->frequency();
    }
}

double TrenResultFactors::getSpeed(const int begin, const int end)
{
    if (begin >= 0 && begin < m_signal->size() &&
        end >= 0 && end < m_signal->size() &&
        begin < end)
    {
        double speed = 0;

        double ox = 0;
        double oy = 0;

        for (int i = begin; i < end; ++i)
        {
            auto x = m_signal->value(0, i);
            auto y = m_signal->value(1, i);

            if (i > begin)
                speed += sqrt(pow((x - ox), 2) + pow((y - oy), 2));

            ox = x;
            oy = y;
        }

        if ((end - begin - 1) > 0)
            return speed / (end - begin - 1) * m_freqency;
    }

    return 0;
}
