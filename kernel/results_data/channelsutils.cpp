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
               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanRitmogram,
                                                                    std::make_tuple(QString("Ритмограмма"),
                                                                                    QString("RR"),
                                                                                    ChannelsDefines::ctRitmogram))
               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynHand1,
                                                                    std::make_tuple(QString("Кистевой силомер 1"),
                                                                                    QString("DH1"),
                                                                                    ChannelsDefines::ctDynamo))
               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynStand1,
                                                                      std::make_tuple(QString("Становой силомер 1"),
                                                                                      QString("SD1"),
                                                                                      ChannelsDefines::ctDynamo))
               , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBreath1,
                                                                        std::make_tuple(QString("Дыхание 1"),
                                                                                        QString("Br1"),
                                                                                        ChannelsDefines::ctBreath))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynPush1,
                                                                          std::make_tuple(QString("Толкатель 1"),
                                                                                          QString("Push1"),
                                                                                          ChannelsDefines::ctDynamo))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynHand2,
                                                                      std::make_tuple(QString("Кистевой силомер 2"),
                                                                                      QString("DH2"),
                                                                                      ChannelsDefines::ctDynamo))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynStand2,
                                                                        std::make_tuple(QString("Становой силомер 2"),
                                                                                        QString("SD2"),
                                                                                        ChannelsDefines::ctDynamo))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBreath2,
                                                                          std::make_tuple(QString("Дыхание 2"),
                                                                                          QString("Br2"),
                                                                                          ChannelsDefines::ctBreath))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynPush2,
                                                                          std::make_tuple(QString("Толкатель 2"),
                                                                                          QString("Push2"),
                                                                                          ChannelsDefines::ctDynamo))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynHand3,
                                                                      std::make_tuple(QString("Кистевой силомер 3"),
                                                                                      QString("DH3"),
                                                                                      ChannelsDefines::ctDynamo))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynStand3,
                                                                        std::make_tuple(QString("Становой силомер 3"),
                                                                                        QString("SD3"),
                                                                                        ChannelsDefines::ctDynamo))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanBreath3,
                                                                          std::make_tuple(QString("Дыхание 3"),
                                                                                          QString("Br3"),
                                                                                          ChannelsDefines::ctBreath))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDynPush2,
                                                                          std::make_tuple(QString("Толкатель 2"),
                                                                                          QString("Push2"),
                                                                                          ChannelsDefines::ctDynamo))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanMyogram,
                                                                          std::make_tuple(QString("Миограмма"),
                                                                                          QString("Myo"),
                                                                                          ChannelsDefines::ctMyogram))

                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTenso1,
                                                                          std::make_tuple(QString("Тензоканал 1"),
                                                                                          QString("T1"),
                                                                                          ChannelsDefines::ctTenso))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTenso2,
                                                                          std::make_tuple(QString("Тензоканал 2"),
                                                                                          QString("T2"),
                                                                                          ChannelsDefines::ctTenso))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTenso3,
                                                                          std::make_tuple(QString("Тензоканал 3"),
                                                                                          QString("T3"),
                                                                                          ChannelsDefines::ctTenso))

                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanJumpSingleHeight,
                                                                    std::make_tuple(QString("Высота одиночного прыжка"),
                                                                                    QString("H"),
                                                                                    ChannelsDefines::ctJumpSingleHeight))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanJumpHeight,
                                                                    std::make_tuple(QString("Высота прыжков"),
                                                                                    QString("H"),
                                                                                    ChannelsDefines::ctJumpHeight))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTeppingData,
                                                                    std::make_tuple(QString("Теппинг тест"),
                                                                                    QString("Тепп.тест"),
                                                                                    ChannelsDefines::ctTeppingData))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanDropTestData,
                                                                    std::make_tuple(QString("Дроп тест"),
                                                                                    QString("Дроп тест"),
                                                                                    ChannelsDefines::ctDropTestData))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanHopping,
                                                                    std::make_tuple(QString("Соскакивание на платформу"),
                                                                                    QString("JOP"),
                                                                                    ChannelsDefines::ctHopping))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTrenResult,
                                                                    std::make_tuple(QString("Результаты сеанса тренинга"),
                                                                                    QString("TrenRes"),
                                                                                    ChannelsDefines::ctTrenResult))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanCrossResult,
                                                                    std::make_tuple(QString("Результаты теста на устойчивость"),
                                                                                    QString("CrossRes"),
                                                                                    ChannelsDefines::ctCrossResult))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanStepOffsetResult,
                                                                    std::make_tuple(QString("Результаты теста \"Ступени\""),
                                                                                    QString("StepOffsRes"),
                                                                                    ChannelsDefines::ctStepOffsetResult))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanEvolventaResult,
                                                                    std::make_tuple(QString("Результаты теста \"Эвольвента\""),
                                                                                    QString("EvolventaRes"),
                                                                                    ChannelsDefines::ctStepOffsetResult))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanTargetTraectory,
                                                                    std::make_tuple(QString("Траектория цели"),
                                                                                    QString("TargetTraectory"),
                                                                                    ChannelsDefines::ctDecartTraectory))
                 , std::pair<QString, ChannelsDefines::ChannelInfo>(ChannelsDefines::chanAnySignalDual,
                                                                    std::make_tuple(QString("Сигнал двухканальный"),
                                                                                    QString("AnySignalDual"),
                                                                                    ChannelsDefines::ctDecartTraectory))
                 })
    , m_formats({
                  std::pair<QString, QString>(ChannelsDefines::ctDecartTraectory, ChannelsDefines::cfDecartCoordinates)
                , std::pair<QString, QString>(ChannelsDefines::ctStabilogram, ChannelsDefines::cfDecartCoordinates)
                , std::pair<QString, QString>(ChannelsDefines::ctBallistogram, ChannelsDefines::cfSinglePositive)
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

}
