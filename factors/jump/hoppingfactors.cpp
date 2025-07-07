#include "hoppingfactors.h"

#include "dataprovider.h"
#include "channelsutils.h"
#include "jumpplatedata.h"
#include "aanalyserapplication.h"

HoppingFactors::HoppingFactors(const QString &testUid,
                               const QString &probeUid,
                               const QString &channelId,
                               const int begin,
                               const int end,
                               QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, begin, end, parent)
{
    if (isValid())
        calculate();
}

QString HoppingFactors::uid() const
{
    return HoppingFactorsDefines::GroupUid;
}

QString HoppingFactors::name() const
{
    return nameAsConst();
}

QString HoppingFactors::nameAsConst()
{
    return QCoreApplication::tr("Показатели соскакивания на платформу");
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
            registerGroup(HoppingFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(HoppingFactorsDefines::JumpHeightUid, HoppingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Высота прыжка"), QCoreApplication::tr("h"), QCoreApplication::tr("м"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(HoppingFactorsDefines::TimeUid, HoppingFactorsDefines::GroupUid,
                           QCoreApplication::tr("Задержка на платформе"), QCoreApplication::tr("t"), QCoreApplication::tr("сек"), 2, 2, FactorsDefines::nsBelow, 12);
}
