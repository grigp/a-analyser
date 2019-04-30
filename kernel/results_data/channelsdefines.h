#ifndef CHANNELSDEFINES_H
#define CHANNELSDEFINES_H

#include <QObject>
#include <QString>

namespace ChannelsDefines
{

/*!
 * \brief Идентификаторы типов каналов
 */
 static QString ctStabilogram = "{1F7A5F8A-FC64-44E2-B2D2-5245D02ED2CA}";
 static QString ctBallistogram = "{98CA38D2-8423-4AE5-9AF3-A47AA58F2616}";

/*!
 * \brief идентификаторы каналов
 */
 static QString chanStab = "{61E814F9-CE64-46F6-B158-46961964BEE9}";
 static QString chanZ = "{41280317-450F-4CF7-B3D9-15E2D0B87469}";


/*!
 * \brief Структура информации о канале ChannelInfo struct
 */
struct ChannelInfo
{
    QString name;
    QString shortName;
    QString type;
    ChannelInfo() {}
    ChannelInfo(std::tuple<QString, QString, QString> params)
        : name(std::get<0>(params))
        , shortName(std::get<1>(params))
        , type(std::get<2>(params))
    {}
};


}

#endif // CHANNELSDEFINES_H
