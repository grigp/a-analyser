#include "droptestfactors.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "jumpplatedata.h"

DropTestFactors::DropTestFactors(const QString &testUid,
                               const QString &probeUid,
                               const QString &channelId,
                               QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

bool DropTestFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool DropTestFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctDropTestData;
}

void DropTestFactors::calculate()
{
    m_jumps.clear();
    int jCount = 0;
    double avgHeight = 0;
    double avgContactTime = 0;
    double avgNoContactTime = 0;

    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), channelId(), baData))
    {
        DropTestData data(baData);

        for (int i = 0; i < data.jumpsCount(); ++i)
        {
            m_jumps.append(data.jump(i));
            avgHeight = avgHeight + data.jump(i).height;
            avgContactTime = avgContactTime + data.jump(i).timeContact;
            avgNoContactTime = avgNoContactTime + data.jump(i).timeNoContact;
        }
        m_time = data.time();

        jCount = data.jumpsCount();
        if (data.jumpsCount() > 0)
        {
            avgHeight = avgHeight / jCount;
            avgContactTime = avgContactTime / jCount;
            avgNoContactTime = avgNoContactTime / jCount;
        }
    }

    addFactor(DropTestFactorsDefines::FullTimeUid, m_time);
    addFactor(DropTestFactorsDefines::JumpsCountUid, jCount);
    addFactor(DropTestFactorsDefines::JumpsHeightAvrUid, avgHeight);
    addFactor(DropTestFactorsDefines::JumpsContactTimeAvrUid, avgContactTime);
    addFactor(DropTestFactorsDefines::JumpsNoContactTimeAvrUid, avgNoContactTime);
}

void DropTestFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(DropTestFactorsDefines::GroupUid, tr("Показатели дроп теста"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::JumpsCountUid, DropTestFactorsDefines::GroupUid,
                           tr("Количество прыжков"), tr("Кол-во"), tr(""), 0, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::FullTimeUid, DropTestFactorsDefines::GroupUid,
                           tr("Общее время"), tr("Время"), tr("сек"), 3, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::JumpsHeightAvrUid, DropTestFactorsDefines::GroupUid,
                           tr("Средняя высота прыжка"), tr("h"), tr("cм"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::JumpsContactTimeAvrUid, DropTestFactorsDefines::GroupUid,
                           tr("Среднее время контактной фазы"), tr("Контакт"), tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(DropTestFactorsDefines::JumpsNoContactTimeAvrUid, DropTestFactorsDefines::GroupUid,
                           tr("Среднее время бесконтактной фазы"), tr("Бесконтакт"), tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
}

int DropTestFactors::jumpsCount() const
{
    return m_jumps.size();
}

SignalsDefines::DropJumpRec DropTestFactors::jump(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_jumps.size());
    return m_jumps.at(idx);
}
