#include "mwstickforcefactors.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "dynamosignal.h"

MWStickForceFactors::MWStickForceFactors(const QString &testUid,
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

QString MWStickForceFactors::uid() const
{
    return MWStickForceFactorsDefines::GroupUid;
}

QString MWStickForceFactors::name() const
{
    return MWStickForceFactorsDefines::GroupName;
}

bool MWStickForceFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool MWStickForceFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    Q_UNUSED(probeUid);
    return channelId == ChannelsDefines::chanMWStickForce;
//    return DataProvider::channelExists(probeUid, channelId) &&
//           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctRitmogram;
}

void MWStickForceFactors::calculate()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), channelId(), baData))
    {
        DynamoSignal signal(baData);

        if (signal.size() > 0)
        {
            //! Начальная и конечная точки
            m_begin = 0;
            m_end = signal.size() - 1;
            if (begin() < end())
            {
                if (begin() > -1)
                    m_begin = begin();
                if (end() > -1)
                    m_end = end();
            }

            m_averageForce = 0;
            m_maxForce = 0;
            for (int i = m_begin; i <= m_end; ++i)
            {
                double v = signal.value(i);
                m_averageForce += v;
                if (v > m_maxForce)
                    m_maxForce = v;
            }

            if (m_end > m_begin)
                m_averageForce /= (m_end - m_begin);
        }
    }

    addFactor(MWStickForceFactorsDefines::MaxForceUid, m_maxForce);
    addFactor(MWStickForceFactorsDefines::AverageForceUid, m_averageForce);
}

void MWStickForceFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(MWStickForceFactorsDefines::GroupUid, MWStickForceFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(MWStickForceFactorsDefines::MaxForceUid, MWStickForceFactorsDefines::GroupUid,
                           tr("Максимальное усилие"), tr("Макс"), tr("кг"), 1, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(MWStickForceFactorsDefines::AverageForceUid, MWStickForceFactorsDefines::GroupUid,
                           tr("Среднее усилие"), tr("Среднее"), tr("кг"), 1, 1, FactorsDefines::nsDual, 12);
}
