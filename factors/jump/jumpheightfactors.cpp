#include "jumpheightfactors.h"

#include "dataprovider.h"
#include "channelsutils.h"
#include "jumpplatedata.h"
#include "aanalyserapplication.h"

#include <QDebug>

JumpHeightFactors::JumpHeightFactors(const QString &testUid,
                                   const QString &probeUid,
                                   const QString &channelId,
                                   QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool JumpHeightFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool JumpHeightFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctJumpHeight;
}

void JumpHeightFactors::calculate()
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), channelId(), baStab))
    {
        JumpHeightData data(baStab);
        m_height = data.height();
    }

    addFactor(JumpHeightFactorsDefines::JumpHeightUid, m_height);
}

void JumpHeightFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(JumpHeightFactorsDefines::GroupUid, tr("Показатели высоты прыжка"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::JumpHeightUid, JumpHeightFactorsDefines::GroupUid,
                           tr("Высота прыжка"), tr("h"), tr("м"), 4, 2, FactorsDefines::nsAbove, 12);
}
