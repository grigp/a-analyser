#include "vectorfactors.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "channelsutils.h"

VectorFactors::VectorFactors(const QString &testUid,
                             const QString &probeUid,
                             const QString &channelId,
                             QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool VectorFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool VectorFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctStabilogram;
}

void VectorFactors::calculate()
{

}

void VectorFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(VectorFactorsDefines::GroupUid, tr("Векторные показатели"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::KFRUid, VectorFactorsDefines::GroupUid,
                           tr("Качество функции равновесия"), tr("КФР"), tr("%"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::NPVUid, VectorFactorsDefines::GroupUid,
                           tr("Нормированная площадь векторограммы"), tr("НПВ"), tr("кв.мм./с"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::KRINDUid, VectorFactorsDefines::GroupUid,
                           tr("Коэф-т резкого изм. напр. движения"), tr("КРИНД"), tr("%"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::VMidUid, VectorFactorsDefines::GroupUid,
                           tr("Линейная скорость средняя"), tr("ЛСС"), tr("мм/сек"), "");
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(VectorFactorsDefines::AmplVUid, VectorFactorsDefines::GroupUid,
                           tr("Амплитуда вариации линейной скорости"), tr("АВЛС"), tr("мм/сек"), "");

}
