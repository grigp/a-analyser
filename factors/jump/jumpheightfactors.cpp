#include "jumpheightfactors.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "jumpplatedata.h"

JumpHeightFactors::JumpHeightFactors(const QString &testUid,
                                     const QString &probeUid,
                                     const QString &channelId,
                                     QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool JumpHeightFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool JumpHeightFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctJumpHeight;
}

void JumpHeightFactors::calculate()
{
    m_jumps.clear();
    int jCount = 0;
    double avgHeight = 0;
    double avgTime = 0;

    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), channelId(), baData))
    {
        JumpHeightData data(baData);

        for (int i = 0; i < data.jumpsCount(); ++i)
        {
            m_jumps.append(data.jump(i));
            avgHeight = avgHeight + data.jump(i).height;
            avgTime = avgTime + data.jump(i).timeContact;
        }
        m_time = data.time();

        jCount = data.jumpsCount();
        if (data.jumpsCount() > 0)
        {
            avgHeight = avgHeight / jCount;
            avgTime = avgTime / jCount;
        }
    }

    addFactor(JumpHeightFactorsDefines::FullTimeUid, m_time);
    addFactor(JumpHeightFactorsDefines::JumpsCountUid, jCount);
    addFactor(JumpHeightFactorsDefines::JumpsHeightAvrUid, avgHeight);
    addFactor(JumpHeightFactorsDefines::JumpsContactTimeAvrUid, avgTime);
}

void JumpHeightFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(JumpHeightFactorsDefines::GroupUid, tr("Показатели высоты прыжков"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::JumpsCountUid, JumpHeightFactorsDefines::GroupUid,
                           tr("Количество прыжков"), tr("Кол-во"), tr(""), 0, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::FullTimeUid, JumpHeightFactorsDefines::GroupUid,
                           tr("Общее время"), tr("Время"), tr("сек"), 3, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::JumpsHeightAvrUid, JumpHeightFactorsDefines::GroupUid,
                           tr("Средняя высота прыжка"), tr("h"), tr("cм"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::JumpsContactTimeAvrUid, JumpHeightFactorsDefines::GroupUid,
                           tr("Среднее время контакта"), tr("Контакт"), tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
}

int JumpHeightFactors::jumpsCount() const
{
    return m_jumps.size();
}

SignalsDefines::JumpRec JumpHeightFactors::jump(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_jumps.size());
    return m_jumps.at(idx);
}
