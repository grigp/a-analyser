#include "jumpheightfactors.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "jumpplatedata.h"

JumpHeightFactors::JumpHeightFactors(const QString &testUid,
                                     const QString &probeUid,
                                     const QString &channelId,
                                     const int begin,
                                     const int end,
                                     QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, begin, end, parent)
{
    if (isValid())
        calculate();
}

QString JumpHeightFactors::uid() const
{
    return JumpHeightFactorsDefines::GroupUid;
}

QString JumpHeightFactors::name() const
{
    return nameAsConst();
}

QString JumpHeightFactors::nameAsConst()
{
    return QCoreApplication::tr("Показатели высоты прыжков");
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
    double maxHeight = 0;
    double avgTime = 0;

    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), channelId(), baData))
    {
        JumpHeightData data(baData);

        for (int i = 0; i < data.jumpsCount(); ++i)
        {
            m_jumps.append(data.jump(i));
            avgHeight = avgHeight + data.jump(i).height;
            if (data.jump(i).height > maxHeight)
                maxHeight = data.jump(i).height;
            avgTime = avgTime + data.jump(i).timeJump;
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
    addFactor(JumpHeightFactorsDefines::JumpsHeightMaxUid, maxHeight);
    addFactor(JumpHeightFactorsDefines::JumpsContactTimeAvrUid, avgTime);
}

void JumpHeightFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(JumpHeightFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::JumpsCountUid, JumpHeightFactorsDefines::GroupUid,
                           QCoreApplication::tr("Количество прыжков"), QCoreApplication::tr("Кол-во"), QCoreApplication::tr(""), 0, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::FullTimeUid, JumpHeightFactorsDefines::GroupUid,
                           QCoreApplication::tr("Общее время"), QCoreApplication::tr("Время"), QCoreApplication::tr("сек"), 3, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::JumpsHeightAvrUid, JumpHeightFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя высота прыжка"), QCoreApplication::tr("h"), QCoreApplication::tr("м"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::JumpsHeightMaxUid, JumpHeightFactorsDefines::GroupUid,
                           QCoreApplication::tr("Максимальная высота прыжка"), QCoreApplication::tr("h"), QCoreApplication::tr("м"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(JumpHeightFactorsDefines::JumpsContactTimeAvrUid, JumpHeightFactorsDefines::GroupUid,
                           QCoreApplication::tr("Среднее время контакта"), QCoreApplication::tr("Контакт"), QCoreApplication::tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
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
