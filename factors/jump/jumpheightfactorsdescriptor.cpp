#include "jumpheightfactorsdescriptor.h"

#include "jumpheightfactors.h"

JumpHeightFactorsDescriptor::JumpHeightFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    JumpHeightFactors::registerFactors();
}

QString JumpHeightFactorsDescriptor::uid()
{
    return JumpHeightFactorsDefines::GroupUid;
}

QString JumpHeightFactorsDescriptor::name()
{
    return JumpHeightFactorsDefines::GroupName;
}

MultiFactor *JumpHeightFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new JumpHeightFactors(testUid, probeUid, channelUid);
}
