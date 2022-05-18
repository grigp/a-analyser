#include "targetfactorsdescriptor.h"

#include "targetfactors.h"

TargetFactorsDescriptor::TargetFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    TargetFactors::registerFactors();
}

QString TargetFactorsDescriptor::uid()
{
    return TargetFactorsDefines::GroupUid;
}

QString TargetFactorsDescriptor::name()
{
    return TargetFactorsDefines::GroupName;
}

MultiFactor *TargetFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new TargetFactors(testUid, probeUid, channelUid);
}
