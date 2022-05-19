#include "jumpheightsinglefactorsdescriptor.h"

#include "jumpheightsinglefactors.h"

JumpHeightSingleFactorsDescriptor::JumpHeightSingleFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    JumpHeightSingleFactors::registerFactors();
}

QString JumpHeightSingleFactorsDescriptor::uid()
{
    return JumpHeightSingleFactorsDefines::GroupUid;
}

QString JumpHeightSingleFactorsDescriptor::name()
{
    return JumpHeightSingleFactorsDefines::GroupName;
}

MultiFactor *JumpHeightSingleFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new JumpHeightSingleFactors(testUid, probeUid, channelUid);
}
