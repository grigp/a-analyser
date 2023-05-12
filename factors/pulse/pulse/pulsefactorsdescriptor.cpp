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
    return PulseFactorsDefines::GroupName;
}

bool PulseFactorsDescriptor::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    return PulseFactors::isValid(testUid, probeUid, channelId);
}

MultiFactor *PulseFactorsDescriptor::caclulate(const QString &testUid, const QString &probeUid, const QString &channelUid)
{
    return new PulseFactors(testUid, probeUid, channelUid);
}
