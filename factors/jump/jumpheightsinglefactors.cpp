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

bool JumpHeightSingleFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool JumpHeightSingleFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctJumpHeight;
}

void JumpHeightSingleFactors::calculate()
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), channelId(), baStab))
    {
        JumpHeightSingleData data(baStab);
        m_height = data.height();
    }

    addFactor(JumpHeightSingleFactorsDefines::JumpHeightUid, m_height);
}

void JumpHeightSingleFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(JumpHeightSingleFactorsDefines::GroupUid, tr("Показатели высоты прыжка"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightSingleFactorsDefines::JumpHeightUid, JumpHeightSingleFactorsDefines::GroupUid,
                           tr("Высота прыжка"), tr("h"), tr("м"), 4, 2, FactorsDefines::nsAbove, 12);
}
