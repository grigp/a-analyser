#include "channelsutils.h"

ChannelsUtils ChannelsUtils::instance()
{
    static ChannelsUtils _instance;
    return _instance;
}

QString ChannelsUtils::channelType(const QString &channelId) const
{
    return m_channels.value(channelId).type;
}

QString ChannelsUtils::channelName(const QString &channelId) const
{
    return m_channels.value(channelId).name;
}

QString ChannelsUtils::channelShortName(const QString &channelId) const
{
    return m_channels.value(channelId).shortName;
}

ChannelsUtils::ChannelsUtils()
    : m_channels({
                 std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStab,
                                                                  std::make_tuple(QString("Стабилографический сигнал"),
                                                                                  QString("Stab"),
                                                                                  ChannelsDefines::ctStabilogram))
               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZ,
                                                                  std::make_tuple(QString("Баллистограмма"),
                                                                                  QString("Z"),
                                                                                  ChannelsDefines::ctBallistogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanJumpHeight,
                                                                    std::make_tuple(QString("Высота прыжка"),
                                                                                    QString("H"),
                                                                                    ChannelsDefines::ctJumpHeight))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanHopping,
                                                                    std::make_tuple(QString("Соскакивание на платформу"),
                                                                                    QString("JOP"),
                                                                                    ChannelsDefines::ctHopping))
                 })
{

}
