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
    return JumpHeightSingleFactors::nameAsConst();
}

bool JumpHeightSingleFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return JumpHeightSingleFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *JumpHeightSingleFactorsDescriptor::caclulate(const QString &testUid,
                                                          const QString &probeUid,
                                                          const QString &channelUid,
                                                          const int begin,
                                                          const int end)
{
    return new JumpHeightSingleFactors(testUid, probeUid, channelUid, begin, end);
}
