#include "pulsefactors.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "ritmogram.h"

PulseFactors::PulseFactors(const QString &testUid,
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

QString PulseFactors::uid() const
{
    return PulseFactorsDefines::GroupUid;
}

QString PulseFactors::name() const
{
    return PulseFactorsDefines::GroupName;
}

bool PulseFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool PulseFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctRitmogram;
}

void PulseFactors::calculate()
{
    QByteArray baPulse;
    if (DataProvider::getChannel(probeUid(), channelId(), baPulse))
    {
        Ritmogram signal(baPulse);

        if (signal.size() > 0)
        {
            //! Средний пульс
            m_pulse = 0;
            for (int i = 0; i < signal.size(); ++i)
                m_pulse += signal.value(i);
            m_pulse /= signal.size();
        }
    }

    addFactor(PulseFactorsDefines::PulseUid, m_pulse);
}

void PulseFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(PulseFactorsDefines::GroupUid, PulseFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::PulseUid, PulseFactorsDefines::GroupUid,
                           tr("Средний пульс"), tr("ЧСС"), tr("уд/мин"), 1, 1, FactorsDefines::nsDual, 12);
}
