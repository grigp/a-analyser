#include "crossfactorsdescriptor.h"

#include "crossfactors.h"

CrossFactorsDescriptor::CrossFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    CrossFactors::registerFactors();
}

QString CrossFactorsDescriptor::uid()
{
    return CrossFactorsDefines::GroupUid;
}

QString CrossFactorsDescriptor::name()
{
    return CrossFactors::nameAsConst();
}

bool CrossFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    return CrossFactors::isValid(testUid, probeUid);
}

MultiFactor *CrossFactorsDescriptor::caclulate(const QString &testUid,
                                               const QString &probeUid,
                                               const QString &channelUid,
                                               const int begin,
                                               const int end)
{
    Q_UNUSED(channelUid);
    Q_UNUSED(begin);
    Q_UNUSED(end);
    return new CrossFactors(testUid, probeUid);
}

