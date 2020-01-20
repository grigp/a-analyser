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
 static QString ctJumpSingleHeight = "{828DC7F7-2E70-4912-9ED5-03AA5A8BE729}";
 static QString ctJumpHeight = "{53BD9EE2-BB3F-4D3E-BD7F-719D7F73155D}";
 static QString ctTeppingData = "{45F90DE1-57B7-4987-BC37-8C6654CBC636}";
 static QString ctDropTestData = "{671EFFCF-4886-490A-B666-7D6ED90847A7}";
 static QString ctHopping = "{77A6BE07-5937-4A8A-8521-08D6C379BDD8}";
 static QString ctTrenResult = "{1FC8CB00-550E-4DF2-B6A3-7758B3867278}";


/*!
 * \brief идентификаторы каналов
 */
 static QString chanStab = "{61E814F9-CE64-46F6-B158-46961964BEE9}";
 static QString chanZ = "{41280317-450F-4CF7-B3D9-15E2D0B87469}";
 static QString chanJumpSingleHeight = "{EF9DF092-7FE0-4F74-9EA6-8BA3318AA91C}";
 static QString chanJumpHeight = "{8FE3810D-2A66-4A2E-8921-8E089E577B6E}";
 static QString chanTeppingData = "{25544CFD-A203-4BE9-8297-5D672D0EC7D5}";
 static QString chanDropTestData = "{74B71080-34A4-4480-80D0-E0D85C11B6B5}";
 static QString chanHopping = "{EF4AD6ED-2D10-497A-AB8D-945CF6C0645E}";

 static QString chanMyo = "{E4342137-B1BF-4F68-BC17-2C337b97A642}";
 static QString chanPulse = "{84736B99-7F4B-494A-9958-D6F908E07C32}";

 static QString chanTrenResult = "{BE102815-C928-4120-919E-4BB21A25EB34}";

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
