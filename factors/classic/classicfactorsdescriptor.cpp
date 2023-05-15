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

bool ClassicFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return ClassicFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *ClassicFactorsDescriptor::caclulate(const QString &testUid,
                                                 const QString &probeUid,
                                                 const QString &channelUid,
                                                 const int begin,
                                                 const int end)
{
    return new ClassicFactors(testUid, probeUid, channelUid, begin, end);
}
