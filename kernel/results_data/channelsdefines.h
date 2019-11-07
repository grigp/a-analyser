#ifndef CHANNELSDEFINES_H
#define CHANNELSDEFINES_H

#include <QObject>
#include <QString>

namespace ChannelsDefines
{

/*!
 * \brief Идентификаторы форматов каналов
 */
static QString cfSinglePositive = "{F0B6F40E-D5BD-4888-9D3C-B370E068945E}";  ///< Одиночный положительный
static QString cfSingleNegative = "{BB902571-208C-4238-8917-249F2FD51DE8}";  ///< Одиночный отрицательный
static QString cfSingleDual = "{1E3D3FD7-8635-404D-95D8-082A5A5061DF}";      ///< Одиночный двунаправленный
static QString cfDecartCoordinates = "{86261A11-D896-45C7-A3E6-600ECF2A9F21}"; ///< декартовы координаты (два канала двунаправленных)
static QString cfNoSignal = "{90e313c6-8805-4235-8981-1fb85d7aea5f}";          ///< Не является сигналом (не может быть использован для поиска)

/*!
 * \brief Идентификаторы типов каналов
 */
 static QString ctStabilogram = "{1F7A5F8A-FC64-44E2-B2D2-5245D02ED2CA}";
 static QString ctBallistogram = "{98CA38D2-8423-4AE5-9AF3-A47AA58F2616}";
 static QString ctJumpHeight = "{828DC7F7-2E70-4912-9ED5-03AA5A8BE729}";
 static QString ctHopping = "{77A6BE07-5937-4A8A-8521-08D6C379BDD8}";


/*!
 * \brief идентификаторы каналов
 */
 static QString chanStab = "{61E814F9-CE64-46F6-B158-46961964BEE9}";
 static QString chanZ = "{41280317-450F-4CF7-B3D9-15E2D0B87469}";
 static QString chanJumpHeight = "{EF9DF092-7FE0-4F74-9EA6-8BA3318AA91C}";
 static QString chanHopping = "{EF4AD6ED-2D10-497A-AB8D-945CF6C0645E}";


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
