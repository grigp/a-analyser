#ifndef CHANNELSUTILS_H
#define CHANNELSUTILS_H

#include <QObject>
#include <QMap>

#include "channelsdefines.h"

class ChannelEnumerator : public QObject
{
public:
    explicit ChannelEnumerator(QObject *parent = nullptr);

    ChannelsDefines::ChannelInfo channelInfo(const QString& chanId);

private:
    QMap<QString, ChannelsDefines::ChannelInfo> m_channels;
};

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

    QString channelType(const QString &channelId);

    QString channelName(const QString &channelId);

    QString channelShortName(const QString &channelId);

    QString channelFormat(const QString &channelId);

    /*!
     * \brief Возвращает идентификатор канала баллистограммы для канала стабилограммы
     * \param channelIdStab - идентификатор канала стабилограммы
     */
    QString zChanForStabChan(const QString &channelIdStab) const;

private:
    ChannelsUtils();
    ChannelsUtils(const ChannelsUtils &rhs) = default;

    ChannelEnumerator *m_channels;
//    QMap<QString, ChannelsDefines::ChannelInfo> m_channels;
    QMap<QString, QString> m_formats; ///< Соответствие типа канала его формату
};

#endif // CHANNELSUTILS_H
