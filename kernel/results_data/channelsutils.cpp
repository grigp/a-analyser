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

QString ChannelsUtils::channelFormat(const QString &channelId) const
{
    auto type = channelType(channelId);
    if (type != "")
    {
        if (m_formats.contains(type))
            return m_formats.value(type);
    }
    return ChannelsDefines::cfNoSignal;
}

QString ChannelsUtils::zChanForStabChan(const QString &channelIdStab) const
{
    if (channelIdStab == ChannelsDefines::chanStab)
        return ChannelsDefines::chanZ;
    return "";
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
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanJumpSingleHeight,
                                                                    std::make_tuple(QString("Высота одиночного прыжка"),
                                                                                    QString("H"),
                                                                                    ChannelsDefines::ctJumpSingleHeight))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanJumpHeight,
                                                                    std::make_tuple(QString("Высота прыжков"),
                                                                                    QString("H"),
                                                                                    ChannelsDefines::ctJumpHeight))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanHopping,
                                                                    std::make_tuple(QString("Соскакивание на платформу"),
                                                                                    QString("JOP"),
                                                                                    ChannelsDefines::ctHopping))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTrenResult,
                                                                    std::make_tuple(QString("Результаты сеанса тренинга"),
                                                                                    QString("TrenRes"),
                                                                                    ChannelsDefines::ctTrenResult))
                 })
    , m_formats({
                  std::pair<QString, QString>(ChannelsDefines::ctStabilogram, ChannelsDefines::cfDecartCoordinates)
                , std::pair<QString, QString>(ChannelsDefines::ctBallistogram, ChannelsDefines::cfSinglePositive)
                , std::pair<QString, QString>(ChannelsDefines::ctJumpSingleHeight, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctJumpHeight, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctHopping, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctTrenResult, ChannelsDefines::cfNoSignal)
                })
{

}
