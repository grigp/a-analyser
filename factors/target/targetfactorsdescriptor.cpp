#include "targetfactorsdescriptor.h"

#include "targetfactors.h"

TargetFactorsDescriptor::TargetFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    TargetFactors::registerFactors();
}

QString TargetFactorsDescriptor::uid()
{
    return TargetFactorsDefines::GroupUid;
}

QString TargetFactorsDescriptor::name()
{
    return TargetFactors::nameAsConst();
}

bool TargetFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return TargetFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *TargetFactorsDescriptor::caclulate(const QString &testUid,
                                                const QString &probeUid,
                                                const QString &channelUid,
                                                const int begin,
                                                const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new TargetFactors(testUid, probeUid);
}
