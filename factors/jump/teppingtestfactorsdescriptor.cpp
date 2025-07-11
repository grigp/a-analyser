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
    return TeppingTestFactors::nameAsConst();
}

bool TeppingTestFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return TeppingTestFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *TeppingTestFactorsDescriptor::caclulate(const QString &testUid,
                                                     const QString &probeUid,
                                                     const QString &channelUid,
                                                     const int begin,
                                                     const int end)
{
    return new TeppingTestFactors(testUid, probeUid, channelUid, begin, end);
}
