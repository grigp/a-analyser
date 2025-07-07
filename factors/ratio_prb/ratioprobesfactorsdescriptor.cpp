#include "ratioprobesfactorsdescriptor.h"

#include "ratioprobesfactors.h"

RatioProbesFactorsDescriptor::RatioProbesFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    RatioProbesFactors::registerFactors();
}

QString RatioProbesFactorsDescriptor::uid()
{
    return RatioProbesFactorsDefines::GroupUid;
}

QString RatioProbesFactorsDescriptor::name()
{
    return RatioProbesFactors::nameAsConst();
}

bool RatioProbesFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(probeUid);
    Q_UNUSED(channelId);
    return RatioProbesFactors::isValid(testUid);
}

MultiFactor *RatioProbesFactorsDescriptor::caclulate(const QString &testUid,
                                                     const QString &probeUid,
                                                     const QString &channelUid,
                                                     const int begin,
                                                     const int end)
{
    Q_UNUSED(probeUid);
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new RatioProbesFactors(testUid);
}
