#include "vectorfactorsdescriptor.h"

#include "vectorfactors.h"

VectorFactorsDescriptor::VectorFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    VectorFactors::registerFactors();
}

QString VectorFactorsDescriptor::uid()
{
    return VectorFactorsDefines::GroupUid;
}

QString VectorFactorsDescriptor::name()
{
    return VectorFactorsDefines::GroupName;
}

MultiFactor *VectorFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new VectorFactors(testUid, probeUid, channelUid);
}
