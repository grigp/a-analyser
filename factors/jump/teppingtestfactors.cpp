#include "teppingtestfactors.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "jumpplatedata.h"

TeppingTestFactors::TeppingTestFactors(const QString &testUid,
                                       const QString &probeUid,
                                       const QString &channelId,
                                       QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool TeppingTestFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool TeppingTestFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctTeppingData;
}

void TeppingTestFactors::calculate()
{
    m_stepsLeftLeg.clear();
    m_stepsRightLeg.clear();
    int sLeftCount = 0;
    int sRightCount = 0;
    double avgTimeLeft = 0;
    double avgTimeRight = 0;

    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), channelId(), baData))
    {
        TeppingTestData data(baData);

        for (int i = 0; i < data.stepsCount(BaseUtils::Left); ++i)
        {
            m_stepsLeftLeg.append(data.step(BaseUtils::Left, i));
            avgTimeLeft = avgTimeLeft + data.step(BaseUtils::Left, i).timeContact;
        }
        sLeftCount = data.stepsCount(BaseUtils::Left);
        for (int i = 0; i < data.stepsCount(BaseUtils::Right); ++i)
        {
            m_stepsRightLeg.append(data.step(BaseUtils::Right, i));
            avgTimeRight = avgTimeRight + data.step(BaseUtils::Right, i).timeContact;
        }
        sRightCount = data.stepsCount(BaseUtils::Right);
        m_time = data.time();

        if (sLeftCount > 0)
            avgTimeLeft = avgTimeLeft / sLeftCount;
        if (sRightCount > 0)
            avgTimeRight = avgTimeRight / sRightCount;
    }

    addFactor(TeppingTestFactorsDefines::FullTimeUid, m_time);
    addFactor(TeppingTestFactorsDefines::StepsCountUid, sLeftCount < sRightCount ? sLeftCount : sRightCount);
    addFactor(TeppingTestFactorsDefines::LeftLegContactTimeAvrUid, avgTimeLeft);
    addFactor(TeppingTestFactorsDefines::RightLegContactTimeAvrUid, avgTimeRight);
}

void TeppingTestFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(TeppingTestFactorsDefines::GroupUid, tr("Показатели теппинг теста"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::StepsCountUid, TeppingTestFactorsDefines::GroupUid,
                           tr("Количество шагов"), tr("Кол-во"), tr(""), 0, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::FullTimeUid, TeppingTestFactorsDefines::GroupUid,
                           tr("Общее время"), tr("Время"), tr("сек"), 3, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::LeftLegContactTimeAvrUid, TeppingTestFactorsDefines::GroupUid,
                           tr("Быстрота одиночного движения левой ноги"), tr("Контакт (лев)"), tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::RightLegContactTimeAvrUid, TeppingTestFactorsDefines::GroupUid,
                           tr("Быстрота одиночного движения правой ноги"), tr("Контакт (прав)"), tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
}

int TeppingTestFactors::stepsCount(BaseUtils::Side side) const
{
    if (side == BaseUtils::Left)
        return m_stepsLeftLeg.size();
    else
    if (side == BaseUtils::Right)
        return m_stepsRightLeg.size();
    return 0;
}

SignalsDefines::TeppingStepRec TeppingTestFactors::step(BaseUtils::Side side, const int idx) const
{
    if (side == BaseUtils::Left)
    {
        Q_ASSERT(idx >= 0 && idx < m_stepsLeftLeg.size());
        return m_stepsLeftLeg.at(idx);
    }
    else
    if (side == BaseUtils::Right)
    {
        Q_ASSERT(idx >= 0 && idx < m_stepsRightLeg.size());
        return m_stepsRightLeg.at(idx);
    }
    return SignalsDefines::TeppingStepRec();
}
