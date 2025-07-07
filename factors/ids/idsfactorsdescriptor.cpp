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
    return IDSFactors::nameAsConst();
}

bool IDSFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return IDSFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *IDSFactorsDescriptor::caclulate(const QString &testUid,
                                             const QString &probeUid,
                                             const QString &channelUid,
                                             const int begin,
                                             const int end)
{
    return new IDSFactors(testUid, probeUid, channelUid, begin, end);
}
