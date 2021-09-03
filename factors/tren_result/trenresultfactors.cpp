#include "trenresultfactors.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "trenresultdata.h"

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

}
