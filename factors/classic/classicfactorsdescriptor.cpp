#include "classicfactorsdescriptor.h"

#include "classicfactors.h"

ClassicFactorsDescriptor::ClassicFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    ClassicFactors::registerFactors();
}

QString ClassicFactorsDescriptor::uid()
{
    return ClassicFactorsDefines::GroupUid;
}

QString ClassicFactorsDescriptor::name()
{
    return ClassicFactorsDefines::GroupName;
}

MultiFactor *ClassicFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new ClassicFactors(testUid, probeUid, channelUid);
}
