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

bool TargetFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return TargetFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *TargetFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    Q_UNUSED(channelUid);
    return new TargetFactors(testUid, probeUid);
}
