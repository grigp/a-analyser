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

bool JumpHeightFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return JumpHeightFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *JumpHeightFactorsDescriptor::caclulate(const QString &testUid,
                                                    const QString &probeUid,
                                                    const QString &channelUid,
                                                    const int begin,
                                                    const int end)
{
    return new JumpHeightFactors(testUid, probeUid, channelUid, begin, end);
}
