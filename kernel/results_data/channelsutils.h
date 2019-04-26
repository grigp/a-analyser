#ifndef CHANNELSUTILS_H
#define CHANNELSUTILS_H

#include <QObject>
#include <QMap>

#include "channelsdefines.h"

/*!
 * \brief Класс утилит для доступа к константам каналов ChannelsUtils class
 */
class ChannelsUtils
{
public:
    static ChannelsUtils instance();

    QString channelType(const QString &channelId) const;

    QString channelName(const QString &channelId) const;

    QString channelShortName(const QString &channelId) const;

private:
    ChannelsUtils();
    ChannelsUtils(const ChannelsUtils &rhs) = default;

    QMap<QString, ChannelsDefines::ChannelInfo> m_channels;
};

#endif // CHANNELSUTILS_H
