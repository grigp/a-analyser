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
 static QString ctDecartTraectory = "{589AC0CD-CC3C-467D-9C48-90223F673ACC}";
 static QString ctStabilogram = "{1F7A5F8A-FC64-44E2-B2D2-5245D02ED2CA}";
 static QString ctBalistogram = "{98CA38D2-8423-4AE5-9AF3-A47AA58F2616}";
 static QString ctRitmogram = "{C211078B-9219-4806-9D1D-52B1A4AC5282}";
 static QString ctDynamo = "{6534BEE8-F805-4E9D-B972-1279E0A3FA3A}";
 static QString ctBreath = "{B3366A97-6EBC-4E6C-B004-AE04A96C3260}";
 static QString ctMyogram = "{0DA6EBB3-15BC-41D6-90F2-59D847A42863}";
 static QString ctWeightPlate = "{50DFB124-CF42-4E14-9A02-82DFBFAAB159}";
 static QString ctADCValues = "{4E00D963-0247-4F30-BB94-1AA72A6938EC}";
 static QString ctBilat = "{3C37787A-C74f-43C8-B0FE-F60D7D4D7504}";

 static QString ctTenso = "{A7D6E553-3BE1-4FDF-A29D-A4DB98F88AFC}";  ///< Виртуальный тензоканал

 static QString ctJumpSingleHeight = "{828DC7F7-2E70-4912-9ED5-03AA5A8BE729}";
 static QString ctJumpHeight = "{53BD9EE2-BB3F-4D3E-BD7F-719D7F73155D}";
 static QString ctTeppingData = "{45F90DE1-57B7-4987-BC37-8C6654CBC636}";
 static QString ctDropTestData = "{671EFFCF-4886-490A-B666-7D6ED90847A7}";
 static QString ctHopping = "{77A6BE07-5937-4A8A-8521-08D6C379BDD8}";
 static QString ctTrenResult = "{1FC8CB00-550E-4DF2-B6A3-7758B3867278}";
 static QString ctTakePutResult = "{5090C76B-B64C-431E-A9EE-FC1D85466C55}";
 static QString ctCrossResult = "{E58B0706-5868-4232-8C78-30F02556BA94}";
 static QString ctStepOffsetResult = "{5A085503-191D-493E-AB99-F44E5277A465}";
 static QString ctEvolventaResult = "{00C24635-99EF-4811-AFEF-DA375AAC332A}";
 static QString ctOctaedronResult = "{2A1C5BE8-4862-4CC8-BE74-47E562722692}";
 static QString ctBoxerDodgingResult = "{79CC9B9F-E327-41A2-811D-51E6A7D3CE9F}";
 static QString ctParticalWeighting = "{73A98FA4-D1CB-466C-8DFF-66ED99236BFE}";


/*!
 * \brief идентификаторы каналов
 */
 static QString chanStab = "{61E814F9-CE64-46F6-B158-46961964BEE9}";
 static QString chanStabLeft = "{5583F847-AA5B-45DE-A35A-2403DCB56ACF}";
 static QString chanStabRight = "{756AEB70-F814-4B7D-B412-F3B5DD776472}";
 static QString chanStab1 = "{6B28D8BA-BD2E-487F-810E-9FEADF4F04C2}";
 static QString chanStab2 = "{9ED91B45-A3EF-4ED5-97EB-70671EF63D6F}";
 static QString chanZ = "{41280317-450F-4CF7-B3D9-15E2D0B87469}";
 static QString chanZLeft = "{C6189CC8-23CF-4697-BB38-4875CDA8C04E}";
 static QString chanZRight = "{504F3104-5028-4AA8-AE81-F60491612DEF}";
 static QString chanZ1 = "{764C536A-D2C7-4271-A89A-F1461C95ECDA}";
 static QString chanZ2 = "{2DE8A98D-03A9-4DF1-AA27-724B3FB9A02B}";
 static QString chanWeight = "{13E17652-5130-4ECD-B049-68C0517872F2}";
 static QString chanRitmogram = "{C9E0639D-D077-4D7D-ABE6-28FC2DE2FA90}";
 static QString chanDynHand1 = "{A8A04F44-C766-495B-8662-BA4A2B0C2D6C}";
 static QString chanDynStand1 = "{AE1F3EB7-684D-41ED-8BA9-44CB608E2043}";
 static QString chanBreath1 = "{77BD2F44-A2E9-4060-8F9D-DE0839894765}";
 static QString chanDynPush1 = "{6A0E07CC-89E2-413F-BA25-458049BF3A38}";
 static QString chanDynHand2 = "{37EF498F-822C-4545-B06C-8251A91CEB30}";
 static QString chanDynStand2 = "{856F89A0-020A-43FE-839A-A2634033AB20}";
 static QString chanBreath2 = "{F62110AF-F47C-4307-916D-D76A0274D12B}";
 static QString chanDynPush2 = "{C2DA4910-785B-4C74-BA7F-749FE0C96F6A}";
 static QString chanDynHand3 = "{7FF1ABDF-EE82-4907-9CDE-73527CB5CAA1}";
 static QString chanDynStand3 = "{D4512319-8B39-4B93-B11F-5F9BE321D37D}";
 static QString chanBreath3 = "{35EC81F9-F56A-409F-B1EB-E84B001E9FBD}";
 static QString chanDynPush3 = "{31C2F1CD-0AEC-4424-9B27-C8C93EBE75D6}";
 static QString chanMyogram = "{DD5FE1AC-9423-46AD-9EB0-50D52EA6D022}";
 static QString chanWeightPlate = "{D252EC6B-56E6-4E03-B532-88B013F89E88}";
 static QString chanADCValues = "{25108273-94B1-44B9-9F5B-850B89F9CF5E}";

 //! Первая платформа
 namespace FirstPlatform
 {
 static QString chanRitmogram = "{91895299-88A4-428E-A2A0-6C5D8B4543E1}";

 static QString chanDynHand1 = "{008E0BA0-6748-4197-8238-4081840FEE09}";
 static QString chanDynStand1 = "{5185D730-5CD0-4F26-B2AC-3F56C4D92541}";
 static QString chanBreath1 = "{837C6798-8B9C-4560-98E2-2755D38FF9B3}";
 static QString chanDynPush1 = "{25ACDA40-B362-42B1-8D00-928AC0C52860}";
 static QString chanDynHand2 = "{509CB4FF-01A5-4671-A3A5-E2C29BB5FCD4}";
 static QString chanDynStand2 = "{A673CA22-B2BF-4644-B581-38FE470F36A1}";
 static QString chanBreath2 = "{5EB8E227-AEE3-431F-A7EE-080FC1F16308}";
 static QString chanDynPush2 = "{964C8093-0ABE-414B-97B7-E564ECD76A6F}";
 static QString chanDynHand3 = "{A48F6BEE-FCA7-4B99-B18F-BF07037A2EBD}";
 static QString chanDynStand3 = "{56BB38F7-11B4-45D7-8F4E-5CDD53D97E8D}";
 static QString chanBreath3 = "{41CC440C-12AF-4DDC-BCF8-763059D5E851}";
 static QString chanDynPush3 = "{02F6E455-146B-465C-9AE7-6C6C14F33BDB}";
 static QString chanMyogram = "{6184597A-AC70-49ED-A642-C2D441015629}";
 }

 //! Вторая платформа
 namespace SecondPlatform
 {
 static QString chanRitmogram = "{A8A80F9B-6429-404F-85BE-4EC88BF401EC}";

 static QString chanDynHand1 = "{9715E506-F7DA-47F2-8EC8-CF55943CEA42}";
 static QString chanDynStand1 = "{2B4F25A4-F5EB-47FD-8D14-AEC716323299}";
 static QString chanBreath1 = "{ACE63F57-90CF-4D66-8F80-53B513604F9A}";
 static QString chanDynPush1 = "{9184AAB8-C2D0-46E3-89FB-E661AEDAE586}";
 static QString chanDynHand2 = "{0F795C5D-0BED-4998-97A7-30127222A44D}";
 static QString chanDynStand2 = "{A66CA8BA-E2C8-4169-B05C-BBB76CA28BFB}";
 static QString chanBreath2 = "{BCD7F0EB-EED3-4C99-9746-F1A6E3A3F1E0}";
 static QString chanDynPush2 = "{CCA96BDF-20C9-4B35-BA01-FFBC10B2094B}";
 static QString chanDynHand3 = "{9A6DE483-4FAD-4EF2-AAE2-B0B0F2E4CB1D}";
 static QString chanDynStand3 = "{C6EBB744-2559-42A6-9699-BFB7131937E5}";
 static QString chanBreath3 = "{A39E1F28-F8EE-4649-8375-EB2DCDA5C90E}";
 static QString chanDynPush3 = "{083EAE20-64B2-41DE-8A21-3B546661EDE8}";
 static QString chanMyogram = "{73837A89-7469-4AC9-9C7C-AECF5BF3288C}";
 }

 static QString chanTargetTraectory = "{28983087-1ED3-422C-9AC3-9805002FA6F2}";
 static QString chanAnySignalDual = "{848E3446-5573-4C5B-8268-0D0A5AADD41E}";

 static QString chanTenso1 = "{C8BC30A3-19FD-45E3-965E-39BB28E51D15}";  ///< Виртуальный тензоканал 1
 static QString chanTenso2 = "{F832E307-F77E-4FE3-8DD8-2A620D6CC9BB}";  ///< Виртуальный тензоканал 2
 static QString chanTenso3 = "{6615ABFD-B646-4CCA-8F24-59B1CC4DABD6}";  ///< Виртуальный тензоканал 3

 static QString chanMWStickForce = "{CEEDFC5E-C432-42BC-A6AD-31D30F2E156C}";  ///< Усилия соревновательной палки

 static QString chanJumpSingleHeight = "{EF9DF092-7FE0-4F74-9EA6-8BA3318AA91C}";
 static QString chanJumpHeight = "{8FE3810D-2A66-4A2E-8921-8E089E577B6E}";
 static QString chanTeppingData = "{25544CFD-A203-4BE9-8297-5D672D0EC7D5}";
 static QString chanDropTestData = "{74B71080-34A4-4480-80D0-E0D85C11B6B5}";
 static QString chanHopping = "{EF4AD6ED-2D10-497A-AB8D-945CF6C0645E}";

 static QString chanTrenResult = "{BE102815-C928-4120-919E-4BB21A25EB34}";
 static QString chanTakePutResult = "{B2DC3947-A399-4621-8C8B-7C0626880AB3}";
 static QString chanCrossResult = "{D0382F32-69DC-41C6-8057-DEf52A743B55}";
 static QString chanStepOffsetResult = "{62720541-7467-466E-9C18-2208AB303280}";
 static QString chanEvolventaResult = "{C3513F54-08B4-4E30-A2E0-D07987FAB84A}";
 static QString chanOctaedronResult = "{2B1AF94A-C4B7-4FA9-A173-1667413C2B99}";
 static QString chanBoxerDodgingResult = "{0B1C5B04-3F5F-4A49-86DC-AC12047AD5CF}";
 static QString chanTriangleResult = "{AF373B61-07FE-47D3-BF88-3D76E817C67A}";

 static QString chanBilat = "{6A5985B5-0954-4877-9309-EB3F6A2AFC91}";

 static QString chanParticalWeighting = "{F41B17B2-B1CB-4BC5-927B-8D6B6F397429}";

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
