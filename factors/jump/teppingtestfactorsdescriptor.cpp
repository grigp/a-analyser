#include "teppingtestfactorsdescriptor.h"

#include "teppingtestfactors.h"

TeppingTestFactorsDescriptor::TeppingTestFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    TeppingTestFactors::registerFactors();
}

QString TeppingTestFactorsDescriptor::uid()
{
    return TeppingTestFactorsDefines::GroupUid;
}

QString TeppingTestFactorsDescriptor::name()
{
    return TeppingTestFactorsDefines::GroupName;
}

MultiFactor *TeppingTestFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new TeppingTestFactors(testUid, probeUid, channelUid);
}
