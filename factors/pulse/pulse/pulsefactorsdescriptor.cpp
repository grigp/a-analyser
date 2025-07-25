#include "pulsefactorsdescriptor.h"

#include "pulsefactors.h"

PulseFactorsDescriptor::PulseFactorsDescriptor(BaseDefines::TestLevel level)
    : MultiFactorDescriptor (level)
{
    PulseFactors::registerFactors();
}

QString PulseFactorsDescriptor::uid()
{
    return PulseFactorsDefines::GroupUid;
}

QString PulseFactorsDescriptor::name()
{
    return PulseFactors::nameAsConst();
}

bool PulseFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return PulseFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *PulseFactorsDescriptor::caclulate(const QString &testUid,
                                               const QString &probeUid,
                                               const QString &channelUid,
                                               const int begin,
                                               const int end)
{
    return new PulseFactors(testUid, probeUid, channelUid, begin, end);
}
