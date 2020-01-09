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

    ///< Роли для данных в переключателе выбора каналов для управления
    enum ChannelSelectRoles
    {
          ChannelUidRole = Qt::UserRole + 0   ///< Идентификатор канала QString
        , SubChanNumRole                      ///< Номер подканала int
    };

    static ChannelsUtils instance();

    QString channelType(const QString &channelId) const;

    QString channelName(const QString &channelId) const;

    QString channelShortName(const QString &channelId) const;

    QString channelFormat(const QString &channelId) const;

    /*!
     * \brief Возвращает идентификатор канала баллистограммы для канала стабилограммы
     * \param channelIdStab - идентификатор канала стабилограммы
     */
    QString zChanForStabChan(const QString &channelIdStab) const;

private:
    ChannelsUtils();
    ChannelsUtils(const ChannelsUtils &rhs) = default;

    QMap<QString, ChannelsDefines::ChannelInfo> m_channels;
    QMap<QString, QString> m_formats; ///< Соответствие типа канала его формату
};

#endif // CHANNELSUTILS_H
