#include "idsfactorsdescriptor.h"

#include "idsfactors.h"

IDSFactorsDescriptor::IDSFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    IDSFactors::registerFactors();
}

QString IDSFactorsDescriptor::uid()
{
    return IDSFactorsDefines::GroupUid;
}

QString IDSFactorsDescriptor::name()
{
    return IDSFactorsDefines::GroupName;
}

MultiFactor *IDSFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new IDSFactors(testUid, probeUid, channelUid);
}