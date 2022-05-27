#include "jumpheightsinglefactors.h"

#include "dataprovider.h"
#include "channelsutils.h"
#include "jumpplatedata.h"
#include "aanalyserapplication.h"

#include <QDebug>

JumpHeightSingleFactors::JumpHeightSingleFactors(const QString &testUid,
                                                 const QString &probeUid,
                                                 const QString &channelId,
                                                 QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

QString JumpHeightSingleFactors::uid() const
{
    return JumpHeightSingleFactorsDefines::GroupUid;
}

QString JumpHeightSingleFactors::name() const
{
    return JumpHeightSingleFactorsDefines::GroupName;
}

bool JumpHeightSingleFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool JumpHeightSingleFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctJumpSingleHeight;
}

void JumpHeightSingleFactors::calculate()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), channelId(), baData))
    {
        JumpHeightSingleData data(baData);
        m_height = data.height();
    }

    addFactor(JumpHeightSingleFactorsDefines::JumpHeightUid, m_height);
}

void JumpHeightSingleFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(JumpHeightSingleFactorsDefines::GroupUid, JumpHeightSingleFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightSingleFactorsDefines::JumpHeightUid, JumpHeightSingleFactorsDefines::GroupUid,
                           tr("Высота прыжка"), tr("h"), tr("м"), 4, 2, FactorsDefines::nsAbove, 12);
}
