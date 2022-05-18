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
    return RatioProbesFactorsDefines::GroupName;
}

MultiFactor *RatioProbesFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(probeUid);
    Q_UNUSED(channelUid);
    return new RatioProbesFactors(testUid);
}
