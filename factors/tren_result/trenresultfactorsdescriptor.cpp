#include "trenresultfactorsdescriptor.h"

#include "trenresultfactors.h"

TrenResultFactorsDescriptor::TrenResultFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    TrenResultFactors::registerFactors();
}

QString TrenResultFactorsDescriptor::uid()
{
    return TrenResultFactorsDefines::GroupUid;
}

QString TrenResultFactorsDescriptor::name()
{
    return TrenResultFactorsDefines::GroupName;
}

MultiFactor *TrenResultFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new TrenResultFactors(testUid, probeUid, channelUid);
}
