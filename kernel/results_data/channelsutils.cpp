#include "channelsutils.h"

#include <QCoreApplication>
#include <QDebug>

ChannelsUtils ChannelsUtils::instance()
{
    static ChannelsUtils _instance;
    return _instance;
}

QString ChannelsUtils::channelType(const QString &channelId)
{
    return m_channels->channelInfo(channelId).type;
}

QString ChannelsUtils::channelName(const QString &channelId)
{
    return m_channels->channelInfo(channelId).name;
}

QString ChannelsUtils::channelShortName(const QString &channelId)
{
    return m_channels->channelInfo(channelId).shortName;
}

QString ChannelsUtils::channelFormat(const QString &channelId)
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
    else
    if (channelIdStab == ChannelsDefines::chanStabLeft)
        return ChannelsDefines::chanZLeft;
    else
    if (channelIdStab == ChannelsDefines::chanStabRight)
        return ChannelsDefines::chanZRight;
    else
    if (channelIdStab == ChannelsDefines::chanStab1)
        return ChannelsDefines::chanZ1;
    else
    if (channelIdStab == ChannelsDefines::chanStab2)
        return ChannelsDefines::chanZ2;

    return "";
}

ChannelsUtils::ChannelsUtils()
//    : m_channels({
//                 std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStab,
//                                                                  std::make_tuple(QString("Стабилографический сигнал"),
//                                                                                  QString("Stab"),
//                                                                                  ChannelsDefines::ctStabilogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStabLeft,
//                                                                  std::make_tuple(QString("Стабилографический сигнал (слева)"),
//                                                                                  QString("StabLeft"),
//                                                                                  ChannelsDefines::ctStabilogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStabRight,
//                                                                  std::make_tuple(QString("Стабилографический сигнал (справа)"),
//                                                                                  QString("StabRight"),
//                                                                                  ChannelsDefines::ctStabilogram))
//                , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStab1,
//                                                                   std::make_tuple(QString("Стабилографический сигнал 1"),
//                                                                                   QString("Stab1"),
//                                                                                   ChannelsDefines::ctStabilogram))
//                , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStab2,
//                                                                   std::make_tuple(QString("Стабилографический сигнал 2"),
//                                                                                   QString("Stab2"),
//                                                                                   ChannelsDefines::ctStabilogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZ,
//                                                                  std::make_tuple(QString("Баллистограмма"),
//                                                                                  QString("Z"),
//                                                                                  ChannelsDefines::ctBalistogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZLeft,
//                                                                  std::make_tuple(QString("Баллистограмма (слева)"),
//                                                                                  QString("ZLeft"),
//                                                                                  ChannelsDefines::ctBalistogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZRight,
//                                                                  std::make_tuple(QString("Баллистограмма (справа)"),
//                                                                                  QString("ZRight"),
//                                                                                  ChannelsDefines::ctBalistogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZ1,
//                                                                  std::make_tuple(QString("Баллистограмма 1"),
//                                                                                  QString("Z1"),
//                                                                                  ChannelsDefines::ctBalistogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZ2,
//                                                                  std::make_tuple(QString("Баллистограмма 2"),
//                                                                                  QString("Z2"),
//                                                                                  ChannelsDefines::ctBalistogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanWeight,
//                                                                  std::make_tuple(QString("Динамика веса"),
//                                                                                  QString("Weight"),
//                                                                                  ChannelsDefines::ctBalistogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanRitmogram,
//                                                                    std::make_tuple(QString("Ритмограмма"),
//                                                                                    QString("RR"),
//                                                                                    ChannelsDefines::ctRitmogram))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynHand1,
//                                                                    std::make_tuple(QString("Кистевой силомер 1"),
//                                                                                    QString("DH1"),
//                                                                                    ChannelsDefines::ctDynamo))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynStand1,
//                                                                      std::make_tuple(QString("Становой силомер 1"),
//                                                                                      QString("SD1"),
//                                                                                      ChannelsDefines::ctDynamo))
//               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBreath1,
//                                                                        std::make_tuple(QString("Дыхание 1"),
//                                                                                        QString("Br1"),
//                                                                                        ChannelsDefines::ctBreath))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynPush1,
//                                                                          std::make_tuple(QString("Толкатель 1"),
//                                                                                          QString("Push1"),
//                                                                                          ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynHand2,
//                                                                      std::make_tuple(QString("Кистевой силомер 2"),
//                                                                                      QString("DH2"),
//                                                                                      ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynStand2,
//                                                                        std::make_tuple(QString("Становой силомер 2"),
//                                                                                        QString("SD2"),
//                                                                                        ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBreath2,
//                                                                          std::make_tuple(QString("Дыхание 2"),
//                                                                                          QString("Br2"),
//                                                                                          ChannelsDefines::ctBreath))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynPush2,
//                                                                          std::make_tuple(QString("Толкатель 2"),
//                                                                                          QString("Push2"),
//                                                                                          ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynHand3,
//                                                                      std::make_tuple(QString("Кистевой силомер 3"),
//                                                                                      QString("DH3"),
//                                                                                      ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynStand3,
//                                                                        std::make_tuple(QString("Становой силомер 3"),
//                                                                                        QString("SD3"),
//                                                                                        ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBreath3,
//                                                                          std::make_tuple(QString("Дыхание 3"),
//                                                                                          QString("Br3"),
//                                                                                          ChannelsDefines::ctBreath))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynPush2,
//                                                                          std::make_tuple(QString("Толкатель 2"),
//                                                                                          QString("Push2"),
//                                                                                          ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanMyogram,
//                                                                          std::make_tuple(QString("Миограмма"),
//                                                                                          QString("Myo"),
//                                                                                          ChannelsDefines::ctMyogram))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanWeightPlate,
//                                                                          std::make_tuple(QString("Данные весовой платформы"),
//                                                                                          QString("WP"),
//                                                                                          ChannelsDefines::ctWeightPlate))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanADCValues,
//                                                                          std::make_tuple(QString("Данные АЦП"),
//                                                                                          QString("ADC"),
//                                                                                          ChannelsDefines::ctADCValues))

//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanRitmogram,
//                                                                    std::make_tuple(QString("Ритмограмма 1"),
//                                                                                    QString("RR1"),
//                                                                                    ChannelsDefines::ctRitmogram))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynHand1,
//                                                                    std::make_tuple(QString("Кистевой силомер первой платформы 1"),
//                                                                                    QString("DH11"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynStand1,
//                                                                    std::make_tuple(QString("Становой силомер первой платформы 1"),
//                                                                                    QString("DS11"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanBreath1,
//                                                                    std::make_tuple(QString("Дыхание первой платформы 1"),
//                                                                                    QString("Br11"),
//                                                                                    ChannelsDefines::ctBreath))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynPush1,
//                                                                    std::make_tuple(QString("Толкатель первой платформы 1"),
//                                                                                    QString("Push11"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynHand2,
//                                                                    std::make_tuple(QString("Кистевой силомер первой платформы 2"),
//                                                                                    QString("DH12"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynStand2,
//                                                                    std::make_tuple(QString("Становой силомер первой платформы 2"),
//                                                                                    QString("DS12"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanBreath2,
//                                                                    std::make_tuple(QString("Дыхание первой платформы 2"),
//                                                                                    QString("Br12"),
//                                                                                    ChannelsDefines::ctBreath))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynPush2,
//                                                                    std::make_tuple(QString("Толкатель первой платформы 2"),
//                                                                                    QString("Push12"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynHand3,
//                                                                    std::make_tuple(QString("Кистевой силомер первой платформы 3"),
//                                                                                    QString("DH13"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynStand3,
//                                                                    std::make_tuple(QString("Становой силомер первой платформы 3"),
//                                                                                    QString("DS13"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanBreath3,
//                                                                    std::make_tuple(QString("Дыхание первой платформы 3"),
//                                                                                    QString("Br13"),
//                                                                                    ChannelsDefines::ctBreath))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynPush3,
//                                                                    std::make_tuple(QString("Толкатель первой платформы 3"),
//                                                                                    QString("Push13"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanMyogram,
//                                                                    std::make_tuple(QString("Миограмма первой платформы"),
//                                                                                    QString("Myo1"),
//                                                                                    ChannelsDefines::ctMyogram))

//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanRitmogram,
//                                                                    std::make_tuple(QString("Ритмограмма 2"),
//                                                                                    QString("RR2"),
//                                                                                    ChannelsDefines::ctRitmogram))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynHand1,
//                                                                    std::make_tuple(QString("Кистевой силомер второй платформы 1"),
//                                                                                    QString("DH21"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynStand1,
//                                                                    std::make_tuple(QString("Становой силомер второй платформы 1"),
//                                                                                    QString("DS21"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanBreath1,
//                                                                    std::make_tuple(QString("Дыхание второй платформы 1"),
//                                                                                    QString("Br21"),
//                                                                                    ChannelsDefines::ctBreath))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynPush1,
//                                                                    std::make_tuple(QString("Толкатель второй платформы 1"),
//                                                                                    QString("Push21"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynHand2,
//                                                                    std::make_tuple(QString("Кистевой силомер второй платформы 2"),
//                                                                                    QString("DH22"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynStand2,
//                                                                    std::make_tuple(QString("Становой силомер второй платформы 2"),
//                                                                                    QString("DS22"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanBreath2,
//                                                                    std::make_tuple(QString("Дыхание второй платформы 2"),
//                                                                                    QString("Br22"),
//                                                                                    ChannelsDefines::ctBreath))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynPush2,
//                                                                    std::make_tuple(QString("Толкатель второй платформы 2"),
//                                                                                    QString("Push22"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynHand3,
//                                                                    std::make_tuple(QString("Кистевой силомер второй платформы 3"),
//                                                                                    QString("DH23"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynStand3,
//                                                                    std::make_tuple(QString("Становой силомер второй платформы 3"),
//                                                                                    QString("DS23"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanBreath3,
//                                                                    std::make_tuple(QString("Дыхание второй платформы 3"),
//                                                                                    QString("Br23"),
//                                                                                    ChannelsDefines::ctBreath))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynPush3,
//                                                                    std::make_tuple(QString("Толкатель второй платформы 3"),
//                                                                                    QString("Push23"),
//                                                                                    ChannelsDefines::ctDynamo))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanMyogram,
//                                                                    std::make_tuple(QString("Миограмма второй платформы"),
//                                                                                    QString("Myo2"),
//                                                                                    ChannelsDefines::ctMyogram))

//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTenso1,
//                                                                          std::make_tuple(QString("Тензоканал 1"),
//                                                                                          QString("T1"),
//                                                                                          ChannelsDefines::ctTenso))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTenso2,
//                                                                          std::make_tuple(QString("Тензоканал 2"),
//                                                                                          QString("T2"),
//                                                                                          ChannelsDefines::ctTenso))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTenso3,
//                                                                          std::make_tuple(QString("Тензоканал 3"),
//                                                                                          QString("T3"),
//                                                                                          ChannelsDefines::ctTenso))

//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanMWStickForce,
//                                                                          std::make_tuple(QString("Усилия на соревновательной палке"),
//                                                                                          QString("MWSF"),
//                                                                                          ChannelsDefines::ctDynamo))


//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanJumpSingleHeight,
//                                                                    std::make_tuple(QString("Высота одиночного прыжка"),
//                                                                                    QString("H"),
//                                                                                    ChannelsDefines::ctJumpSingleHeight))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanJumpHeight,
//                                                                    std::make_tuple(QString("Высота прыжков"),
//                                                                                    QString("H"),
//                                                                                    ChannelsDefines::ctJumpHeight))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTeppingData,
//                                                                    std::make_tuple(QString("Теппинг тест"),
//                                                                                    QString("Тепп.тест"),
//                                                                                    ChannelsDefines::ctTeppingData))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDropTestData,
//                                                                    std::make_tuple(QString("Дроп тест"),
//                                                                                    QString("Дроп тест"),
//                                                                                    ChannelsDefines::ctDropTestData))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanHopping,
//                                                                    std::make_tuple(QString("Соскакивание на платформу"),
//                                                                                    QString("JOP"),
//                                                                                    ChannelsDefines::ctHopping))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTrenResult,
//                                                                    std::make_tuple(QString("Результаты сеанса тренинга"),
//                                                                                    QString("TrenRes"),
//                                                                                    ChannelsDefines::ctTrenResult))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTakePutResult,
//                                                                    std::make_tuple(QString("Результаты захвата - укладки"),
//                                                                                    QString("TakePutRes"),
//                                                                                    ChannelsDefines::ctTakePutResult))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanCrossResult,
//                                                                    std::make_tuple(QString("Результаты теста на устойчивость"),
//                                                                                    QString("CrossRes"),
//                                                                                    ChannelsDefines::ctCrossResult))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStepOffsetResult,
//                                                                    std::make_tuple(QString("Результаты теста \"Ступени\""),
//                                                                                    QString("StepOffsRes"),
//                                                                                    ChannelsDefines::ctStepOffsetResult))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanEvolventaResult,
//                                                                    std::make_tuple(QString("Результаты теста \"Эвольвента\""),
//                                                                                    QString("EvolventaRes"),
//                                                                                    ChannelsDefines::ctEvolventaResult))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanOctaedronResult,
//                                                                    std::make_tuple(QString("Результаты тренажера \"Октаэдр\""),
//                                                                                    QString("OctaedronRes"),
//                                                                                    ChannelsDefines::ctOctaedronResult))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBoxerDodgingResult,
//                                                                    std::make_tuple(QString("Результаты тренажера с уклонением"),
//                                                                                    QString("OctaedronRes"),
//                                                                                    ChannelsDefines::ctBoxerDodgingResult))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTargetTraectory,
//                                                                    std::make_tuple(QString("Траектория цели"),
//                                                                                    QString("TargetTraectory"),
//                                                                                    ChannelsDefines::ctDecartTraectory))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanAnySignalDual,
//                                                                    std::make_tuple(QString("Сигнал двухканальный"),
//                                                                                    QString("AnySignalDual"),
//                                                                                    ChannelsDefines::ctDecartTraectory))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBilat,
//                                                                    std::make_tuple(QString("Билатеральный режим"),
//                                                                                    QString("Bilat"),
//                                                                                    ChannelsDefines::ctBilat))
//                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanParticalWeighting,
//                                                                    std::make_tuple(QString("Взвешивание"),
//                                                                                    QString("Weight"),
//                                                                                    ChannelsDefines::ctParticalWeighting))
//                 })
//, m_formats({
    : m_formats({
                  std::pair<QString, QString>(ChannelsDefines::ctDecartTraectory, ChannelsDefines::cfDecartCoordinates)
                , std::pair<QString, QString>(ChannelsDefines::ctStabilogram, ChannelsDefines::cfDecartCoordinates)
                , std::pair<QString, QString>(ChannelsDefines::ctBalistogram, ChannelsDefines::cfSinglePositive)
                , std::pair<QString, QString>(ChannelsDefines::ctRitmogram, ChannelsDefines::cfSinglePositive)
                , std::pair<QString, QString>(ChannelsDefines::ctDynamo, ChannelsDefines::cfSinglePositive)
                , std::pair<QString, QString>(ChannelsDefines::ctBreath, ChannelsDefines::cfSingleDual)
                , std::pair<QString, QString>(ChannelsDefines::ctMyogram, ChannelsDefines::cfSinglePositive)
                , std::pair<QString, QString>(ChannelsDefines::ctTenso, ChannelsDefines::cfSinglePositive)
                , std::pair<QString, QString>(ChannelsDefines::ctJumpSingleHeight, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctJumpHeight, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctTeppingData, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctDropTestData, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctHopping, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctTrenResult, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctCrossResult, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctStepOffsetResult, ChannelsDefines::cfNoSignal)
                , std::pair<QString, QString>(ChannelsDefines::ctEvolventaResult, ChannelsDefines::cfNoSignal)
                })
{
    m_channels = new ChannelEnumerator();

}


ChannelEnumerator::ChannelEnumerator(QObject *parent)
    : QObject(parent),
      m_channels({
                 std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStab,
                                                                  std::make_tuple(QString(QCoreApplication::tr("Стабилографический сигнал")),
                                                                                  QString(QCoreApplication::tr("Stab")),
                                                                                  ChannelsDefines::ctStabilogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStabLeft,
                                                                    std::make_tuple(QString(QCoreApplication::tr("Стабилографический сигнал (слева)")),
                                                                                    QString(QCoreApplication::tr("StabLeft")),
                                                                                    ChannelsDefines::ctStabilogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStabRight,
                                                                    std::make_tuple(QString(QCoreApplication::tr("Стабилографический сигнал (справа)")),
                                                                                    QString(QCoreApplication::tr("StabRight")),
                                                                                    ChannelsDefines::ctStabilogram))
                  , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStab1,
                                                                     std::make_tuple(QString(QCoreApplication::tr("Стабилографический сигнал 1")),
                                                                                     QString(QCoreApplication::tr("Stab1")),
                                                                                     ChannelsDefines::ctStabilogram))
                  , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStab2,
                                                                     std::make_tuple(QString(QCoreApplication::tr("Стабилографический сигнал 2")),
                                                                                     QString(QCoreApplication::tr("Stab2")),
                                                                                     ChannelsDefines::ctStabilogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZ,
                                                                    std::make_tuple(QString(QCoreApplication::tr("Баллистограмма")),
                                                                                    QString(QCoreApplication::tr("Z")),
                                                                                    ChannelsDefines::ctBalistogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZLeft,
                                                                    std::make_tuple(QString(QCoreApplication::tr("Баллистограмма (слева)")),
                                                                                    QString(QCoreApplication::tr("ZLeft")),
                                                                                    ChannelsDefines::ctBalistogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZRight,
                                                                    std::make_tuple(QString(QCoreApplication::tr("Баллистограмма (справа)")),
                                                                                    QString(QCoreApplication::tr("ZRight")),
                                                                                    ChannelsDefines::ctBalistogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZ1,
                                                                    std::make_tuple(QString(QCoreApplication::tr("Баллистограмма 1")),
                                                                                    QString(QCoreApplication::tr("Z1")),
                                                                                    ChannelsDefines::ctBalistogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanZ2,
                                                                    std::make_tuple(QString(QCoreApplication::tr("Баллистограмма 2")),
                                                                                    QString(QCoreApplication::tr("Z2")),
                                                                                    ChannelsDefines::ctBalistogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanWeight,
                                                                    std::make_tuple(QString(QCoreApplication::tr("Динамика веса")),
                                                                                    QString(QCoreApplication::tr("Weight")),
                                                                                    ChannelsDefines::ctBalistogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanRitmogram,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Ритмограмма")),
                                                                                      QString(QCoreApplication::tr("RR")),
                                                                                      ChannelsDefines::ctRitmogram))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynHand1,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Кистевой силомер 1")),
                                                                                      QString(QCoreApplication::tr("DH1")),
                                                                                      ChannelsDefines::ctDynamo))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynStand1,
                                                                        std::make_tuple(QString(QCoreApplication::tr("Становой силомер 1")),
                                                                                        QString(QCoreApplication::tr("SD1")),
                                                                                        ChannelsDefines::ctDynamo))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBreath1,
                                                                          std::make_tuple(QString(QCoreApplication::tr("Дыхание 1")),
                                                                                          QString(QCoreApplication::tr("Br1")),
                                                                                          ChannelsDefines::ctBreath))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynPush1,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Толкатель 1")),
                                                                                            QString(QCoreApplication::tr("Push1")),
                                                                                            ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynHand2,
                                                                        std::make_tuple(QString(QCoreApplication::tr("Кистевой силомер 2")),
                                                                                        QString(QCoreApplication::tr("DH2")),
                                                                                        ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynStand2,
                                                                          std::make_tuple(QString(QCoreApplication::tr("Становой силомер 2")),
                                                                                          QString(QCoreApplication::tr("SD2")),
                                                                                          ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBreath2,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Дыхание 2")),
                                                                                            QString(QCoreApplication::tr("Br2")),
                                                                                            ChannelsDefines::ctBreath))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynPush2,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Толкатель 2")),
                                                                                            QString(QCoreApplication::tr("Push2")),
                                                                                            ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynHand3,
                                                                        std::make_tuple(QString(QCoreApplication::tr("Кистевой силомер 3")),
                                                                                        QString(QCoreApplication::tr("DH3")),
                                                                                        ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynStand3,
                                                                          std::make_tuple(QString(QCoreApplication::tr("Становой силомер 3")),
                                                                                          QString(QCoreApplication::tr("SD3")),
                                                                                          ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBreath3,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Дыхание 3")),
                                                                                            QString(QCoreApplication::tr("Br3")),
                                                                                            ChannelsDefines::ctBreath))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynPush2,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Толкатель 2")),
                                                                                            QString(QCoreApplication::tr("Push2")),
                                                                                            ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanMyogram,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Миограмма")),
                                                                                            QString(QCoreApplication::tr("Myo")),
                                                                                            ChannelsDefines::ctMyogram))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanWeightPlate,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Данные весовой платформы")),
                                                                                            QString(QCoreApplication::tr("WP")),
                                                                                            ChannelsDefines::ctWeightPlate))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanADCValues,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Данные АЦП")),
                                                                                            QString(QCoreApplication::tr("ADC")),
                                                                                            ChannelsDefines::ctADCValues))

                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanRitmogram,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Ритмограмма 1")),
                                                                                      QString(QCoreApplication::tr("RR1")),
                                                                                      ChannelsDefines::ctRitmogram))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynHand1,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Кистевой силомер первой платформы 1")),
                                                                                      QString(QCoreApplication::tr("DH11")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynStand1,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Становой силомер первой платформы 1")),
                                                                                      QString(QCoreApplication::tr("DS11")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanBreath1,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Дыхание первой платформы 1")),
                                                                                      QString(QCoreApplication::tr("Br11")),
                                                                                      ChannelsDefines::ctBreath))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynPush1,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Толкатель первой платформы 1")),
                                                                                      QString(QCoreApplication::tr("Push11")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynHand2,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Кистевой силомер первой платформы 2")),
                                                                                      QString(QCoreApplication::tr("DH12")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynStand2,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Становой силомер первой платформы 2")),
                                                                                      QString(QCoreApplication::tr("DS12")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanBreath2,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Дыхание первой платформы 2")),
                                                                                      QString(QCoreApplication::tr("Br12")),
                                                                                      ChannelsDefines::ctBreath))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynPush2,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Толкатель первой платформы 2")),
                                                                                      QString(QCoreApplication::tr("Push12")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynHand3,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Кистевой силомер первой платформы 3")),
                                                                                      QString(QCoreApplication::tr("DH13")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynStand3,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Становой силомер первой платформы 3")),
                                                                                      QString(QCoreApplication::tr("DS13")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanBreath3,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Дыхание первой платформы 3")),
                                                                                      QString(QCoreApplication::tr("Br13")),
                                                                                      ChannelsDefines::ctBreath))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanDynPush3,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Толкатель первой платформы 3")),
                                                                                      QString(QCoreApplication::tr("Push13")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::FirstPlatform::chanMyogram,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Миограмма первой платформы")),
                                                                                      QString(QCoreApplication::tr("Myo1")),
                                                                                      ChannelsDefines::ctMyogram))

                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanRitmogram,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Ритмограмма 2")),
                                                                                      QString(QCoreApplication::tr("RR2")),
                                                                                      ChannelsDefines::ctRitmogram))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynHand1,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Кистевой силомер второй платформы 1")),
                                                                                      QString(QCoreApplication::tr("DH21")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynStand1,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Становой силомер второй платформы 1")),
                                                                                      QString(QCoreApplication::tr("DS21")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanBreath1,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Дыхание второй платформы 1")),
                                                                                      QString(QCoreApplication::tr("Br21")),
                                                                                      ChannelsDefines::ctBreath))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynPush1,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Толкатель второй платформы 1")),
                                                                                      QString(QCoreApplication::tr("Push21")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynHand2,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Кистевой силомер второй платформы 2")),
                                                                                      QString(QCoreApplication::tr("DH22")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynStand2,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Становой силомер второй платформы 2")),
                                                                                      QString(QCoreApplication::tr("DS22")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanBreath2,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Дыхание второй платформы 2")),
                                                                                      QString(QCoreApplication::tr("Br22")),
                                                                                      ChannelsDefines::ctBreath))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynPush2,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Толкатель второй платформы 2")),
                                                                                      QString(QCoreApplication::tr("Push22")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynHand3,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Кистевой силомер второй платформы 3")),
                                                                                      QString(QCoreApplication::tr("DH23")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynStand3,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Становой силомер второй платформы 3")),
                                                                                      QString(QCoreApplication::tr("DS23")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanBreath3,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Дыхание второй платформы 3")),
                                                                                      QString(QCoreApplication::tr("Br23")),
                                                                                      ChannelsDefines::ctBreath))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanDynPush3,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Толкатель второй платформы 3")),
                                                                                      QString(QCoreApplication::tr("Push23")),
                                                                                      ChannelsDefines::ctDynamo))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::SecondPlatform::chanMyogram,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Миограмма второй платформы")),
                                                                                      QString(QCoreApplication::tr("Myo2")),
                                                                                      ChannelsDefines::ctMyogram))

                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTenso1,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Тензоканал 1")),
                                                                                            QString(QCoreApplication::tr("T1")),
                                                                                            ChannelsDefines::ctTenso))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTenso2,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Тензоканал 2")),
                                                                                            QString(QCoreApplication::tr("T2")),
                                                                                            ChannelsDefines::ctTenso))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTenso3,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Тензоканал 3")),
                                                                                            QString(QCoreApplication::tr("T3")),
                                                                                            ChannelsDefines::ctTenso))

                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanMWStickForce,
                                                                            std::make_tuple(QString(QCoreApplication::tr("Усилия на соревновательной палке")),
                                                                                            QString(QCoreApplication::tr("MWSF")),
                                                                                            ChannelsDefines::ctDynamo))


                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanJumpSingleHeight,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Высота одиночного прыжка")),
                                                                                      QString(QCoreApplication::tr("H")),
                                                                                      ChannelsDefines::ctJumpSingleHeight))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanJumpHeight,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Высота прыжков")),
                                                                                      QString(QCoreApplication::tr("H")),
                                                                                      ChannelsDefines::ctJumpHeight))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTeppingData,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Теппинг тест")),
                                                                                      QString(QCoreApplication::tr("Тепп.тест")),
                                                                                      ChannelsDefines::ctTeppingData))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDropTestData,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Дроп тест")),
                                                                                      QString(QCoreApplication::tr("Дроп тест")),
                                                                                      ChannelsDefines::ctDropTestData))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanHopping,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Соскакивание на платформу")),
                                                                                      QString(QCoreApplication::tr("JOP")),
                                                                                      ChannelsDefines::ctHopping))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTrenResult,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Результаты сеанса тренинга")),
                                                                                      QString(QCoreApplication::tr("TrenRes")),
                                                                                      ChannelsDefines::ctTrenResult))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTakePutResult,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Результаты захвата - укладки")),
                                                                                      QString(QCoreApplication::tr("TakePutRes")),
                                                                                      ChannelsDefines::ctTakePutResult))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanCrossResult,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Результаты теста на устойчивость")),
                                                                                      QString(QCoreApplication::tr("CrossRes")),
                                                                                      ChannelsDefines::ctCrossResult))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStepOffsetResult,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Результаты теста \"Ступени\"")),
                                                                                      QString(QCoreApplication::tr("StepOffsRes")),
                                                                                      ChannelsDefines::ctStepOffsetResult))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanEvolventaResult,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Результаты теста \"Эвольвента\"")),
                                                                                      QString(QCoreApplication::tr("EvolventaRes")),
                                                                                      ChannelsDefines::ctEvolventaResult))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanOctaedronResult,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Результаты тренажера \"Октаэдр\"")),
                                                                                      QString(QCoreApplication::tr("OctaedronRes")),
                                                                                      ChannelsDefines::ctOctaedronResult))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBoxerDodgingResult,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Результаты тренажера с уклонением")),
                                                                                      QString(QCoreApplication::tr("OctaedronRes")),
                                                                                      ChannelsDefines::ctBoxerDodgingResult))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTargetTraectory,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Траектория цели")),
                                                                                      QString(QCoreApplication::tr("TargetTraectory")),
                                                                                      ChannelsDefines::ctDecartTraectory))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanAnySignalDual,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Сигнал двухканальный")),
                                                                                      QString(QCoreApplication::tr("AnySignalDual")),
                                                                                      ChannelsDefines::ctDecartTraectory))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBilat,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Билатеральный режим")),
                                                                                      QString(QCoreApplication::tr("Bilat")),
                                                                                      ChannelsDefines::ctBilat))
                   , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanParticalWeighting,
                                                                      std::make_tuple(QString(QCoreApplication::tr("Взвешивание")),
                                                                                      QString(QCoreApplication::tr("Weight")),
                                                                                      ChannelsDefines::ctParticalWeighting))
                 })
{

}

ChannelsDefines::ChannelInfo ChannelEnumerator::channelInfo(const QString &chanId)
{
    return m_channels.value(chanId);
}

