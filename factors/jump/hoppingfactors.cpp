#include "hoppingfactors.h"

#include "dataprovider.h"
#include "channelsutils.h"
#include "jumpplatedata.h"
#include "aanalyserapplication.h"

HoppingFactors::HoppingFactors(const QString &testUid,
                               const QString &probeUid,
                               const QString &channelId,
                               QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool HoppingFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool HoppingFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctHopping;
}

void HoppingFactors::calculate()
{
    QByteArray baHD;
    if (DataProvider::getChannel(probeUid(), channelId(), baHD))
    {
        HoppingData data(baHD);
        m_height = data.height();
        m_time = data.time();
    }

    addFactor(HoppingFactorsDefines::JumpHeightUid, m_height);
    addFactor(HoppingFactorsDefines::TimeUid, m_time);
}

void HoppingFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(HoppingFactorsDefines::GroupUid, HoppingFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(HoppingFactorsDefines::JumpHeightUid, HoppingFactorsDefines::GroupUid,
                           tr("Высота прыжка"), tr("h"), tr("м"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(HoppingFactorsDefines::TimeUid, HoppingFactorsDefines::GroupUid,
                           tr("Задержка на платформе"), tr("t"), tr("сек"), 2, 2, FactorsDefines::nsBelow, 12);
}
