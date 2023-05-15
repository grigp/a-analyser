#include "droptestfactorsdescriptor.h"

#include "droptestfactors.h"

DropTestFactorsDescriptor::DropTestFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    DropTestFactors::registerFactors();
}

QString DropTestFactorsDescriptor::uid()
{
    return DropTestFactorsDefines::GroupUid;
}

QString DropTestFactorsDescriptor::name()
{
    return DropTestFactorsDefines::GroupName;
}

bool DropTestFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return DropTestFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *DropTestFactorsDescriptor::caclulate(const QString &testUid,
                                                  const QString &probeUid,
                                                  const QString &channelUid,
                                                  const int begin,
                                                  const int end)
{
    return new DropTestFactors(testUid, probeUid, channelUid, begin, end);
}
