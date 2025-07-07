#include "teppingtestfactors.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "jumpplatedata.h"

#include <QDebug>

TeppingTestFactors::TeppingTestFactors(const QString &testUid,
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

QString TeppingTestFactors::uid() const
{
    return TeppingTestFactorsDefines::GroupUid;
}

QString TeppingTestFactors::name() const
{
    return nameAsConst();
}

QString TeppingTestFactors::nameAsConst()
{
    return QCoreApplication::tr("Показатели теппинг теста");
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
    double avgContactLeft = 0;
    double avgNoContactLeft = 0;
    double avgContactRight = 0;
    double avgNoContactRight = 0;
    double avgTemp = 0;

    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), channelId(), baData))
    {
        TeppingTestData data(baData);

        for (int i = 0; i < data.stepsCount(BaseDefines::Left); ++i)
        {
            m_stepsLeftLeg.append(data.step(BaseDefines::Left, i));
            avgContactLeft = avgContactLeft + data.step(BaseDefines::Left, i).timeContact;
            avgNoContactLeft = avgNoContactLeft + data.step(BaseDefines::Left, i).timeNoContact;
        }
        sLeftCount = data.stepsCount(BaseDefines::Left);

        for (int i = 0; i < data.stepsCount(BaseDefines::Right); ++i)
        {
            m_stepsRightLeg.append(data.step(BaseDefines::Right, i));
            avgContactRight = avgContactRight + data.step(BaseDefines::Right, i).timeContact;
            avgNoContactRight = avgNoContactRight + data.step(BaseDefines::Right, i).timeNoContact;
        }
        sRightCount = data.stepsCount(BaseDefines::Right);
        m_time = data.time();

        if (sLeftCount > 0)
        {
            avgContactLeft = avgContactLeft / sLeftCount;
            avgNoContactLeft = avgNoContactLeft / sLeftCount;
        }
        if (sRightCount > 0)
        {
            avgContactRight = avgContactRight / sRightCount;
            avgNoContactRight = avgNoContactRight / sRightCount;
        }

        avgTemp = (data.stepsCount(BaseDefines::Left) + data.stepsCount(BaseDefines::Right)) / m_time * 60;
    }

    addFactor(TeppingTestFactorsDefines::FullTimeUid, m_time);
    addFactor(TeppingTestFactorsDefines::StepsCountUid, sLeftCount + sRightCount); //);sLeftCount < sRightCount ? sLeftCount : sRightCount);
    addFactor(TeppingTestFactorsDefines::LeftLegNoContactTimeAvrUid, avgNoContactLeft);
    addFactor(TeppingTestFactorsDefines::RightLegNoContactTimeAvrUid, avgNoContactRight);
    addFactor(TeppingTestFactorsDefines::LeftLegContactTimeAvrUid, avgContactLeft);
    addFactor(TeppingTestFactorsDefines::RightLegContactTimeAvrUid, avgContactRight);
    addFactor(TeppingTestFactorsDefines::TempAvrUid, avgTemp);
}

void TeppingTestFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(TeppingTestFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::StepsCountUid, TeppingTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Количество шагов"), QCoreApplication::tr("Кол-во"), QCoreApplication::tr(""), 0, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::FullTimeUid, TeppingTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Общее время"), QCoreApplication::tr("Время"), QCoreApplication::tr("сек"), 3, 1, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::LeftLegNoContactTimeAvrUid, TeppingTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Быстрота одиночного движения левой ноги"), QCoreApplication::tr("Быстрота (лев)"), QCoreApplication::tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::RightLegNoContactTimeAvrUid, TeppingTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Быстрота одиночного движения правой ноги"), QCoreApplication::tr("Быстрота (прав)"), QCoreApplication::tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::LeftLegContactTimeAvrUid, TeppingTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время контакта левой ноги"), QCoreApplication::tr("Контакт (лев)"), QCoreApplication::tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::RightLegContactTimeAvrUid, TeppingTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время контакта правой ноги"), QCoreApplication::tr("Контакт (прав)"), QCoreApplication::tr("cек"), 4, 2, FactorsDefines::nsAbove, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TeppingTestFactorsDefines::TempAvrUid, TeppingTestFactorsDefines::GroupUid,
                           QCoreApplication::tr("Темп шагов"), QCoreApplication::tr("темп"), QCoreApplication::tr("шагов/мин"), 2, 2, FactorsDefines::nsAbove, 12);
}

int TeppingTestFactors::stepsCount(BaseDefines::Side side) const
{
    if (side == BaseDefines::Left)
        return m_stepsLeftLeg.size();
    else
    if (side == BaseDefines::Right)
        return m_stepsRightLeg.size();
    return 0;
}

SignalsDefines::TeppingStepRec TeppingTestFactors::step(BaseDefines::Side side, const int idx) const
{
    if (side == BaseDefines::Left)
    {
        Q_ASSERT(idx >= 0 && idx < m_stepsLeftLeg.size());
        return m_stepsLeftLeg.at(idx);
    }
    else
    if (side == BaseDefines::Right)
    {
        Q_ASSERT(idx >= 0 && idx < m_stepsRightLeg.size());
        return m_stepsRightLeg.at(idx);
    }
    return SignalsDefines::TeppingStepRec();
}
